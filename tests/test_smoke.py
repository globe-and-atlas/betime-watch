from pathlib import Path
import subprocess
import json
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
def test_host_behavior():
 subprocess.run(['python3',str(ROOT/'execution/build.py'),'--host-only'],cwd=ROOT,check=True)
def test_authentic_icon_resources():
 package=json.loads((ROOT/'watchface/package.json').read_text())
 assert package['pebble']['targetPlatforms']==['emery','gabbro','diorite','flint','basalt','chalk']
 for name in ['power','calendar','clock']:
  with Image.open(ROOT/f'watchface/resources/images/{name}.png') as im:
   assert im.size==(32,32)
   colors=list(im.convert('RGBA').get_flattened_data());assert any(p[3]==0 for p in colors);assert any(p[3]==255 for p in colors)
   assert all(p[3] in [0,255] and all(c in [0,85,170,255] for c in p[:3]) for p in colors)
 notices=(ROOT/'THIRD_PARTY_NOTICES.txt').read_bytes();assert notices==(ROOT/'watchface/resources/raw/notices.txt').read_bytes();assert b'Haiku, Inc.' in notices and b'Akshay Agarwal' in notices
