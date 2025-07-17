import os
import google.generativeai as genai
from PIL import Image
from dotenv import load_dotenv
from pathlib import Path

# Load environment variables from a .env file in the project's root
dotenv_path = Path(__file__).resolve().parent.parent / '.env'
load_dotenv(dotenv_path=dotenv_path)

# Get the API key from the environment
API_KEY = os.getenv("GOOGLE_API_KEY")

# Configure the genai library only if the key is valid
if API_KEY and API_KEY != "YOUR_API_KEY_HERE":
    try:
        genai.configure(api_key=API_KEY)
    except Exception as e:
        print(f"Warning: An error occurred during Gemini configuration: {e}")
        API_KEY = None # Invalidate key on config error
else:
    # This message will now appear when the module is first imported if the key is missing.
    print("Warning: GOOGLE_API_KEY is not set or is a placeholder in .env. Captioning will be skipped.")
    API_KEY = None # Ensure key is None if not set correctly

def generate_caption(image_path: str) -> str:
    """
    Generates a caption for a local image file using the Gemini Pro Vision model.
    """
    # Check the API_KEY variable directly instead of calling a non-existent function.
    if not API_KEY:
        # Silently return a placeholder if the API key was never configured.
        return f"placeholder caption for {Path(image_path).stem}"

    try:
        print(f"  Generating caption for {os.path.basename(image_path)}...")
        img = Image.open(image_path)
        
        model = genai.GenerativeModel('gemini-1.5-flash')
        prompt = "Describe this image for a LoRA training dataset. Be concise but descriptive. Focus on the subject, style, and composition."
        
        response = model.generate_content([prompt, img])
        
        caption = response.text.strip().replace('\n', ' ')
        return caption

    except Exception as e:
        print(f"  Error generating caption for {os.path.basename(image_path)}: {e}")
        return f"placeholder caption for {Path(image_path).stem}"