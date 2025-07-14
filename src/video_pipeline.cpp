#include "video_pipeline.h"
#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

VideoProcessor::VideoProcessor(
    const std::filesystem::path& input_path,
    const std::filesystem::path& output_path,
    int total_frames,
    int frame_size
) : 
    input_path_(input_path),
    output_dir_(output_path),
    total_frames_(total_frames),
    frame_size_(frame_size)
    {

    }

    void VideoProcessor::process() {
        // Initialize to null pointer.
        AVFormatContext* format_context = nullptr;

        // Tries to open file and returns error in case of failure.
        if (avformat_open_input(&format_context, this->input_path_.c_str(), nullptr, nullptr) != 0) {
            std::cerr << "Error: Could not open input file." << this->input_path_ <<std::endl;
            return; // Exit if we can't open the function.
        }

        std::cout << "Successfully opened video file." <<std::endl;

        // Reads packets from the file to get stream information. 
        if (avformat_find_stream_info(format_context, nullptr) < 0) {
            std::cerr << "Error: Could not find stream information." << std::endl;
            return;
        }
            // Find the first video stream
            int video_stream_index = -1;
            for (unsigned int i = 0; i < format_context-> nb_streams; ++i) {
                AVCodecParameters* codec_params = format_context->streams[i]->codecpar;

                if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
                    video_stream_index = i ;
                    break; 
                }
            }

            if (video_stream_index == -1 ) {
                std::cerr << "Error: Could not find a video stream in the input file." << std::endl;
                return;
            }

            std::cout << "Found video stream at index " << video_stream_index << std::endl;
        
        }


        // Find and set up the decoder.
        AVCodecParameters* codec_params = format_context->streams[video_stream_index]->codecpar;

        // Find the registered decoder for the codec ID.
        const AVCodec* decoder = avcodec_find_decoder(codec_params->codec_id);
        if (!decoder) {
            std::cerr << "Error: Unsupported codec!" << std::endl;
            avformat_close_input(&format_context);
            return;
        }

        AVCodecContext* codec_context = avcodec_alloc_context3(decoder);
        if (!codec_context) {
            std::cerr << "Error: Could not allocate a decoding context." << std::endl;
            avformat_close_input(&format_context);
            return;
        }

        if (avcodec_parameters_to_context(codec_context, codec_params) < 0) {
            std::cerr << "Error: Could not copy codec parameters to decoder context." << std::endl;
            avcodec_free_context(&codec_context);
            avformat_close_input(&format_context);
            return;
        }

        if (avcodec_open2(codec_context, decoder, nullptr) < 0) {
            std::cerr << "Error: Could not open the decoder." << std::endl;
            avcodec_free_context(&codec_context);
            avformat_close_input(&format_context);
            return;
        }
        std::cout << "Successfully initialized decoder." << std::endl;

        
        // Freecontext when done to avoid memory leaks:
        avcodec_free_context(&codec_context);
        avformat_close_input(&format_context);

    }