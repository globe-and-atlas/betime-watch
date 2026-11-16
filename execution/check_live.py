"""Observe production seconds and battery-service updates in a native emulator."""
import argparse,json,time
from PIL import Image
from capture_native import ROOT,OUT,command,monitors,capture

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--platform',required=True,choices=['emery','gabbro']);args=parser.parse_args();p=args.platform
 if monitors():raise RuntimeError('Shared emulator is already active')
 round_screen=p=='gabbro';width,height=(260,260) if round_screen else (200,228)
 template=Image.open(ROOT/f'media/{p}-default.png').convert('RGB')
 # Compare invariant pixels from the exact fixture rendition to reject stale foregrounds.
 tab=(38,43,124,58) if round_screen else (14,31,100,47)
 battery=(45,211,99,228) if round_screen else (10,206,64,222)
 seconds=(161,211,215,228) if round_screen else (136,206,190,222)
 expected=template.crop(tab).tobytes()
 def read_frame():
  previous=None;stable=0
  for _ in range(40):
   im=capture();pixels=im.tobytes();stable=stable+1 if previous==pixels else 0;previous=pixels
   if stable>=2 and im.size==(width,height) and im.crop(tab).tobytes()==expected:return im
   time.sleep(.12)
  raise RuntimeError('Complete production BeTime frame not observed')
 def set_battery(percent):
  command(['pebble','emu-battery','--emulator',p,'--percent',str(percent)]);time.sleep(1.5);return read_frame()
 install=['pebble','install','--emulator',p,str(ROOT/'dist/BeTime-0.1.0.pbw')]
 try:
  command(install);time.sleep(1);command(install);first=set_battery(80);time.sleep(1.2);second=read_frame()
  assert first.crop(seconds).tobytes()!=second.crop(seconds).tobytes(),'Live seconds did not change'
  lower=set_battery(40);assert first.crop(battery).tobytes()!=lower.crop(battery).tobytes(),'Live battery reading did not change'
  first.save(OUT/f'{p}-production-80.png');lower.save(OUT/f'{p}-production-40.png');(OUT/f'{p}-live-verification.json').write_text(json.dumps({'platform':p,'foreground':True,'seconds_changed':True,'battery_changed':True},indent=2)+'\n');print(p,'live seconds and battery PASS')
 finally:
  if monitors():command(['pebble','emu-battery','--emulator',p,'--percent','100']);command(['pebble','kill'])
if __name__=='__main__':main()
