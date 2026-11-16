"""Resize authentic icons and map opaque pixels to Pebble's 64-color gamut."""
from pathlib import Path
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
def main():
 for name in ['Power','Calendar','Clock']:
  im=Image.open(ROOT/'media/source-icons'/f'{name}.png').convert('RGBA')
  if im.size!=(32,32):im=im.resize((32,32),Image.Resampling.LANCZOS)
  pixels=[tuple(round(v/85)*85 for v in p[:3])+(255 if p[3]>=128 else 0,) for p in im.get_flattened_data()]
  im.putdata(pixels);im.save(ROOT/'watchface/resources/images'/f'{name.lower()}.png')
if __name__=='__main__':main()
