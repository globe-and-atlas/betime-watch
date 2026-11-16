"""Publish exact reviewed artifact once; verify public bytes separately.
Run with the installed Pebble tool Python environment for SDK authentication.
"""
from pathlib import Path
from contextlib import ExitStack
import argparse,hashlib,io,json,mimetypes,sys,time,zipfile
import requests
from PIL import Image
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'execution'))
from publish_watchface_updates import account_mapping,fetch,listing,PublishCommand
ROOT=Path(__file__).resolve().parents[1];STATE=ROOT/'.tmp/store';API='https://appstore-api.repebble.com'
def sha(data):return hashlib.sha256(data).hexdigest()
def save(path,value):
 path.parent.mkdir(parents=True,exist_ok=True);temporary=path.with_suffix('.new');temporary.write_text(json.dumps(value,indent=2)+'\n');temporary.replace(path)
def validate():
 pbw=ROOT/'dist/BeTime-0.1.0.pbw';report=json.loads((ROOT/'.tmp/production-verification.json').read_text());assert sha(pbw.read_bytes())==report['sha256']
 with zipfile.ZipFile(pbw) as z:
  app=json.loads(z.read('appinfo.json'));assert app['targetPlatforms']==['emery','gabbro'];assert app['uuid']==report['uuid']=='f4d2048a-62f7-4642-9997-5a6967da1ff8';assert app['versionLabel']=='0.1.0'
 assets=sorted((ROOT/'media/store').glob('emery_*'))+sorted((ROOT/'media/store').glob('gabbro_*'));assert len(assets)==10
 for platform,size in [('emery',(200,228)),('gabbro',(260,260))]:
  group=[p for p in assets if p.name.startswith(platform)];assert len(group)==5;assert group[0].suffix=='.gif'
  for p in group:
   with Image.open(p) as im:
    assert im.size==size
    if p==group[0]:assert im.n_frames==7
 return pbw,report,assets

def publish():
 pbw,report,assets=validate();receipt=ROOT/'release/store-release.json';guard=STATE/'inflight.json'
 if receipt.exists():print('Receipt exists: skipping upload');return
 if guard.exists():raise RuntimeError('Prior write outcome requires reconciliation; no retry')
 token,mapping=account_mapping();assert not PublishCommand._lookup_app_id_case_insensitive(mapping,report['uuid']),'UUID already exists; reconcile before upload'
 data={'name':'BeTime','type':'watchface','expectedUuid':report['uuid'],'version':'0.1.0','description':(ROOT/'media/description.txt').read_text(),'source':'','visible':'true','isPublished':'true','releaseNotes':'Initial release for Pebble Time 2 and Round 2: Haiku icons, six desktop palettes, custom colors, and 12/24-hour time.'}
 save(guard,{'sha256':report['sha256'],'uuid':report['uuid'],'started_at':time.time()})
 with ExitStack() as stack:
  paths=[('pbwFile',pbw),('iconSmall',ROOT/'media/store/icon-small.png'),('iconLarge',ROOT/'media/store/icon-large.png')]+[('screenshots_'+p.name.split('_')[0],p) for p in assets]
  files=[(key,(p.name,stack.enter_context(p.open('rb')),mimetypes.guess_type(p.name)[0] or 'application/octet-stream')) for key,p in paths]
  try:r=requests.post(API+'/api/dashboard/apps',headers={'Authorization':'Bearer '+token},data=data,files=files,timeout=(15,300))
  except requests.RequestException:raise RuntimeError('Ambiguous POST outcome; guard retained, no retry') from None
 if not r.ok:
  save(STATE/'error.json',{'status':r.status_code,'body':r.text[:1000]});raise RuntimeError(f'Upload HTTP{r.status_code}; guard retained')
 payload=r.json();save(STATE/'response.json',payload);app_id=PublishCommand._extract_app_id(payload);assert app_id
 result={'app_id':app_id,'name':'BeTime','version':'0.1.0','url':f'https://apps.repebble.com/{app_id}','sha256':report['sha256'],'public_verified':False,'screenshot_results':payload.get('screenshotResults',{})};save(receipt,result);guard.unlink()
 assert not result['screenshot_results'].get('failed'),'Media failed; do not repeat release'
 print(result['url'])
def verify():
 pbw,report,assets=validate();receipt=ROOT/'release/store-release.json';result=json.loads(receipt.read_text());page=listing(result['app_id']);urls=[u for u in page['downloads'] if '/0.1.0/' in u];assert urls and sha(fetch(urls[0]))==report['sha256'];assert page['screenshots'][0].endswith('.gif')
 # Public SSR exposes selected platform gallery plus the other platform cover.
 for platform in ['emery','gabbro']:
  cover=next(u for u in page['screenshots'] if '/'+platform+'/' in u);assert cover.endswith('.gif');assert sha(fetch(cover))==sha((ROOT/f'media/store/{platform}_01_motion-colors.gif').read_bytes())
 galleries=json.loads((STATE/'ui-gallery.json').read_text());uploaded=[{'filename':path.name,'url':url} for platform in ['emery','gabbro'] for path,url in zip([p for p in assets if p.name.startswith(platform)],galleries[platform],strict=True)];assert len(uploaded)==10
 matched=[]
 for path in assets:
  item=next(x for x in uploaded if x['filename']==path.name)
  url=item['url'];url=API+url if url.startswith('/') else url
  remote=fetch(url)
  if path.suffix == '.gif':
   assert sha(remote) == sha(path.read_bytes())
   comparison = 'exact GIF bytes'
  else:
   with Image.open(io.BytesIO(remote)) as image, Image.open(path) as source:
    assert image.size == source.size
    public_rgb = image.convert('RGB')
    native_rgb = source.convert('RGB')
    # Store PNG processing can quantize the small icon artwork.
    # Require exact clock, desktop, text, layout and screenshot dimensions.
    round_screen = path.name.startswith('gabbro_')
    centers = (72, 130, 188) if round_screen else (37, 100, 163)
    icon_top = 163 if round_screen else 158
    changed = 0
    pairs = zip(public_rgb.get_flattened_data(), native_rgb.get_flattened_data(), strict=True)
    for index, (public_pixel, native_pixel) in enumerate(pairs):
     if public_pixel == native_pixel:
      continue
     y, x = divmod(index, image.width)
     assert icon_top <= y < icon_top + 32, path.name
     assert any(center - 16 <= x < center + 16 for center in centers), path.name
     changed += 1
    assert changed <= image.width * image.height * .005, path.name
    comparison = 'exact outside icon quantization' if changed else 'exact RGB pixels'
  item = {'filename': path.name, 'url': url, 'comparison': comparison}
  if path.suffix == '.png':
   item['quantized_icon_pixels'] = changed
  matched.append(item)
 save(STATE/'public-proof.json',{**page,'pbw_exact':True,'both_animated_covers_exact':True,'matched_assets':matched});result['public_verified']=True;save(receipt,result);print('PUBLIC VERIFIED',result['url'])
if __name__=='__main__':
 a=argparse.ArgumentParser();a.add_argument('action',choices=['validate','publish','verify']);args=a.parse_args()
 if args.action=='validate':validate();print('ASSETS VALID')
 elif args.action=='publish':publish()
 else:verify()
