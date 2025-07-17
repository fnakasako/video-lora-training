#include "video_pipeline.h"
#include <iostream>
#include <vector>
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

// FFmpeg headers
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

// --- Private Helper Function ---
void save_frame_as_png(AVFrame* frame, int dest_width, int dest_height, bool center_crop, int frame_number, const std::filesystem::path& output_dir, const std::string& base_filename) {
    std::string filename = base_filename + "_frame_" + std::to_string(frame_number) + ".png";
    std::filesystem::path full_output_path = output_dir / filename;

    int src_w = frame->width;
    int src_h = frame->height;
    uint8_t* const* src_data = frame->data;

    if (center_crop) {
        int crop_size = std::min(src_w, src_h);
        int crop_x = (src_w - crop_size) / 2;
        int crop_y = (src_h - crop_size) / 2;
        crop_x &= ~1;
        crop_y &= ~1;
        uint8_t* temp_src_data[4];
        temp_src_data[0] = frame->data[0] + crop_y * frame->linesize[0] + crop_x;
        temp_src_data[1] = frame->data[1] + (crop_y / 2) * frame->linesize[1] + (crop_x / 2);
        temp_src_data[2] = frame->data[2] + (crop_y / 2) * frame->linesize[2] + (crop_x / 2);
        src_data = temp_src_data;
        src_w = crop_size;
        src_h = crop_size;
    }

    SwsContext* sws_ctx = sws_getContext(src_w, src_h, (AVPixelFormat)frame->format, dest_width, dest_height, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws_ctx) {
        std::cerr << "Error: Could not initialize the software scaler." << std::endl;
        return;
    }

    std::vector<uint8_t> rgb_buffer(dest_width * dest_height * 3);
    uint8_t* dest_data[1] = { rgb_buffer.data() };
    int dest_linesize[1] = { dest_width * 3 };
    sws_scale(sws_ctx, src_data, frame->linesize, 0, src_h, dest_data, dest_linesize);
    stbi_write_png(full_output_path.c_str(), dest_width, dest_height, 3, rgb_buffer.data(), dest_width * 3);
    sws_freeContext(sws_ctx);
}

// --- Class Implementation ---
VideoProcessor::VideoProcessor(
    const std::filesystem::path& input_path,
    const std::filesystem::path& output_dir,
    bool center_crop,
    int total_frames,
    int frame_width,
    int frame_height
) : 
    input_path_(input_path),
    output_dir_(output_dir),
    center_crop_(center_crop),
    total_frames_(total_frames),
    frame_width_(frame_width),
    frame_height_(frame_height)
{
    // Constructor body is empty.
}

void VideoProcessor::process() {
    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, this->input_path_.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Error: Could not open input file " << this->input_path_ << std::endl;
        return;
    }
    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        std::cerr << "Error: Could not find stream information." << std::endl;
        avformat_close_input(&format_context);
        return;
    }

    int video_stream_index = -1;
    AVCodecParameters* codec_params = nullptr;
    AVStream* video_stream = nullptr;
    for (unsigned int i = 0; i < format_context->nb_streams; ++i) {
        if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            codec_params = format_context->streams[i]->codecpar;
            video_stream = format_context->streams[i];
            break;
        }
    }
    if (video_stream_index == -1) { /* ... error handling ... */ avformat_close_input(&format_context); return; }

    const AVCodec* decoder = avcodec_find_decoder(codec_params->codec_id);
    if (!decoder) { /* ... error handling ... */ avformat_close_input(&format_context); return; }

    AVCodecContext* codec_context = avcodec_alloc_context3(decoder);
    if (!codec_context) { /* ... error handling ... */ avformat_close_input(&format_context); return; }
    if (avcodec_parameters_to_context(codec_context, codec_params) < 0) { /* ... error handling ... */ avcodec_free_context(&codec_context); avformat_close_input(&format_context); return; }
    if (avcodec_open2(codec_context, decoder, nullptr) < 0) { /* ... error handling ... */ avcodec_free_context(&codec_context); avformat_close_input(&format_context); return; }

    // Determine output dimensions. Use original video's dimensions if not specified.
    const int out_width = (this->frame_width_ > 0) ? this->frame_width_ : codec_context->width;
    const int out_height = (this->frame_height_ > 0) ? this->frame_height_ : codec_context->height;

    double interval_sec = 0.0;
    if (this->total_frames_ > 0) {
        double duration_sec = (double)format_context->duration / AV_TIME_BASE;
        if (duration_sec > 0) {
            interval_sec = duration_sec / this->total_frames_;
        }
    }
    
    double next_save_time_sec = 0.0;
    int saved_frame_count = 0;
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    if (!packet || !frame) { /* ... error handling ... */ avcodec_free_context(&codec_context); avformat_close_input(&format_context); return; }

    std::string base_filename = this->input_path_.stem().string();
    while (av_read_frame(format_context, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_context, packet) == 0) {
                while (avcodec_receive_frame(codec_context, frame) == 0) {
                    double frame_timestamp_sec = (double)frame->pts * av_q2d(video_stream->time_base);
                    if (frame_timestamp_sec >= next_save_time_sec) {
                        // Pass the correct output width and height to the save function
                        save_frame_as_png(frame, out_width, out_height, this->center_crop_, saved_frame_count + 1, this->output_dir_, base_filename);
                        next_save_time_sec += interval_sec;
                        saved_frame_count++;
                        if (this->total_frames_ > 0 && saved_frame_count >= this->total_frames_) {
                            goto end_loop;
                        }
                    }
                }
            }
        }
        av_packet_unref(packet);
    }
end_loop:
    std::cout << "Finished processing video. Saved " << saved_frame_count << " frames." << std::endl;
    av_packet_free(&packet);
    av_frame_free(&frame);
    avcodec_free_context(&codec_context);
    avformat_close_input(&format_context);
}
