"""Verify production identity, target binaries, and absence of fixture controls."""
from pathlib import Path
import hashlib
import json
import subprocess
import zipfile
ROOT=Path(__file__).resolve().parents[1]
def main():
 package=json.loads((ROOT/'watchface/package.json').read_text());version=package['version'];target=ROOT/'dist'/f'BeTime-{version}.pbw'
 platforms=['emery','gabbro','diorite','flint','basalt','chalk']
 assert package['pebble']['targetPlatforms']==platforms
 with zipfile.ZipFile(target) as bundle:
  info=json.loads(bundle.read('appinfo.json'));assert info['uuid']==package['pebble']['uuid'];assert info['versionLabel']==version;assert info['targetPlatforms']==platforms
  expected={key:10000+i for i,key in enumerate(package['pebble']['messageKeys'])};assert info['appKeys']==expected
  for platform in platforms:
   assert bundle.read(platform+'/pebble-app.bin')==(ROOT/'watchface/build'/platform/'pebble-app.bin').read_bytes()
   resources=bundle.read(platform+'/app_resources.pbpack');assert (ROOT/'THIRD_PARTY_NOTICES.txt').read_bytes() in resources
   ids=(ROOT/f'watchface/build/{platform}/src/resource_ids.auto.h').read_text()
   for resource in ['ICON_POWER','ICON_CALENDAR','ICON_CLOCK','THIRD_PARTY_NOTICES']:assert 'RESOURCE_ID_'+resource in ids
 assert 'pebble-js-app.js' in bundle.namelist()
 nm=Path.home()/'Library/Application Support/Pebble SDK/SDKs/current/toolchain/arm-none-eabi/bin/arm-none-eabi-nm'
 for platform in platforms:
  result=subprocess.run([str(nm),str(ROOT/'watchface/build'/platform/'pebble-app.elf')],capture_output=True,text=True,check=True)
  symbols={line.split()[-1] for line in result.stdout.splitlines() if line.split()}
  assert not symbols.intersection({'fh','fm','fs','fb','proof','fixture_steps','fixture_epoch'}),'Fixture control in production ELF'
 report={'version':version,'uuid':info['uuid'],'targets':info['targetPlatforms'],'sha256':hashlib.sha256(target.read_bytes()).hexdigest()};(ROOT/'.tmp/production-verification.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS:',json.dumps(report))
if __name__=='__main__':main()
