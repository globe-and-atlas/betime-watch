"""Assemble the default and palette sheets from native emulator PNGs."""
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
ROOT=Path(__file__).resolve().parents[1]
FONT='/System/Library/Fonts/Supplemental/Arial.ttf'
def main():
 base='#202329';canvas=Image.new('RGB',(1060,630),base);d=ImageDraw.Draw(canvas);heading=ImageFont.truetype(FONT,30);small=ImageFont.truetype(FONT,19)
 d.text((530,30),'BeTime · native emulator renders',font=heading,fill='white',anchor='mm')
 for x,p,label in [(256,'emery','Pebble Time 2'),(782,'gabbro','Pebble Round 2')]:
  im=Image.open(ROOT/f'media/{p}-default.png').convert('RGB');assert im.size==((200,228) if p=='emery' else (260,260));im=im.resize((im.width*2,im.height*2),Image.Resampling.NEAREST);canvas.paste(im,(x-im.width//2,54+(520-im.height)//2));d.text((x,604),label,font=small,fill='white',anchor='mm')
 canvas.save(ROOT/'media/native-comparison.png')
 palette=Image.new('RGB',(900,650),base);d=ImageDraw.Draw(palette)
 for i,name in enumerate(['Classic blue','Midnight','Teal','Plum','Graphite','Monochrome desktop']):
  x=150+300*(i%3);y=18+324*(i//3);case='default' if i==0 else f'palette-{i}';im=Image.open(ROOT/f'media/gabbro-{case}.png').convert('RGB');palette.paste(im,(x-130,y));d.text((x,y+279),name,font=small,fill='white',anchor='mm')
 palette.save(ROOT/'media/native-palettes.png')
if __name__=='__main__':main()
