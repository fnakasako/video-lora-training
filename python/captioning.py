import os 
import google.generativeai as genai
from PIL import Image

try:
    genai.configure(api_key=os.environ["GOOGLE_API_KEY"])
except:
    print("Error: Google API key is not set in environment.")

def generate_caption(image_path: str) -> str:
    """
    Generates a caption for local image file using the Gemini Pro Vision model.

    Args:
        image_path: The path to the image file.
    Returns:
        A string containing the generated caption, or a placeholder on error.
    """
    try:
        # Check if the API key was succesfully configured
        if not genai.get_key():
            raise ValueError("API key not configured.")
        
        print(f"   Generating caption for {os.path.basename(image_path)}...")

        img = Image.open(image_path)

        # Use the gemini-pro-vision model for image understanding
        model = genai.GenerativeModel('gemini-pro-vision')

        # The prompt guides the model to generate a descriptive caption suitable for training.
        prompt = "Describe this image for a LoRA training dataset. Be concise but descriptive. Focus on" \
        "the subject, style, and composition."

        response = model.generate_content([prompt, img])

        # Clean up the response text
        caption = response.text.strip().replace('\n', ' ')
        return caption
    
    except Exception as e:
        print(f"   Error generating caption for {os.path.basename(image_path)}: {e}")
        # Return a placeholder caption if the aPI calls fails
        return f"a high quality image of {os.path.basename(image_path)}"