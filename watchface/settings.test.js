var assert=require('assert'),Module=require('module'),path=require('path');
var original=Module._load,keys={};
require('./package.json').pebble.messageKeys.forEach(function(key,i){keys[key]=10000+i;});
var actual=path.join(__dirname,'node_modules/@rebble/clay/src/js/index.js'),config;
Module._load=function(name){
 if(name==='message_keys')return keys;
 if(name==='@rebble/clay'){var Clay=original.call(this,actual);return function(items,custom,opts){config=items;return new Clay(items,custom,opts);};}
 return original.apply(this,arguments);
};
var events={},storage={},sent=[];
global.localStorage={getItem:function(k){return storage[k]||null;},setItem:function(k,v){storage[k]=v;}};
global.Pebble={addEventListener:function(k,fn){(events[k]||(events[k]=[])).push(fn);},getAccountToken:function(){return 'test';},getWatchToken:function(){return 'test';},getActiveWatchInfo:function(){return {platform:'gabbro',model:'gabbro',language:'en_US',firmware:{major:4,minor:33,patch:1}};},sendAppMessage:function(v,ok){sent.push(v);if(ok)ok();},openURL:function(url){assert(url.indexOf('data:text/html')===0);}};
var settings=require('./src/pkjs/index');
function fire(name,event){(events[name]||[]).forEach(function(fn){fn(event);});}
assert.deepStrictEqual(settings.normalize({}),settings.defaults);
for(var p=0;p<=6;p++)assert.strictEqual(settings.normalize({Palette:String(p)}).Palette,p);
[null,'',false,-1,7,'oops',1.5].forEach(function(v){assert.strictEqual(settings.normalize({Palette:v}).Palette,0);});
for(var mode=0;mode<3;mode++)assert.strictEqual(settings.normalize({TimeFormat:String(mode)}).TimeFormat,mode);
for(var mode=0;mode<5;mode++)assert.strictEqual(settings.normalize({DateFormat:String(mode)}).DateFormat,mode);
assert.strictEqual(settings.normalize({TextColor:0}).TextColor,0);assert.strictEqual(settings.normalize({AccentColor:0x1000000}).AccentColor,0xFFAA00);
assert.strictEqual(settings.normalize({RightTile:9}).RightTile,2);
fire('ready');assert.strictEqual(sent.length,0);fire('showConfiguration');
['','CANCELLED','invalid'].forEach(function(response){fire('webviewclosed',{response:response});});assert.strictEqual(sent.length,0);
fire('webviewclosed',{response:encodeURIComponent(JSON.stringify({Palette:{value:'6'},Background:{value:0xFFFFAA},WindowColor:{value:0x000055},TextColor:{value:0xFFFFFF},AccentColor:{value:0xFFAA55},TimeFormat:{value:'2'},DateFormat:{value:'3'},LeftTile:{value:'3'},MiddleTile:{value:'4'},RightTile:{value:'0'}}))});
assert.deepStrictEqual(sent[0],{Palette:6,Background:0xFFFFAA,WindowColor:0x000055,TextColor:0xFFFFFF,AccentColor:0xFFAA55,TimeFormat:2,DateFormat:3,LeftTile:3,MiddleTile:4,RightTile:0,TemperatureUnit:0,WorldCity:1});
var saved=storage['clay-settings'];fire('webviewclosed',{response:'CANCELLED'});assert.strictEqual(storage['clay-settings'],saved);fire('ready');assert.deepStrictEqual(sent[1],sent[0]);
Object.keys(keys).forEach(function(key){if(keys[key]<=10011)assert(config.some(function(item){return item.messageKey===key;}));});
require('fs').writeFileSync(path.join(__dirname,'../.tmp/clay-save.json'),JSON.stringify(sent[0]));
var data=require('./src/pkjs/desktop-data');
var stamp=Date.UTC(2026,2,28,18,0)/1000;
var london=data.worldMessage(1,stamp);
assert.deepStrictEqual(london,{ZoneCity:1,ZoneOffset:0,ZoneNextOffset:60,
 ZoneChangeAt:Date.UTC(2026,2,29,1,0)/1000,ZoneUntil:stamp+36*3600});
assert.strictEqual(data.worldMessage(8,stamp).ZoneOffset,330);
assert.deepStrictEqual(data.weatherMessage({current:{time:stamp-300,temperature_2m:17.8,weather_code:2}},stamp),
 {WeatherAt:stamp-300,Temperature:178,WeatherKind:1});
assert.strictEqual(data.weatherMessage({current:{time:stamp-3601,temperature_2m:17.8,weather_code:2}},stamp),null);
var requests=[];
Object.defineProperty(global,'navigator',{configurable:true,value:{geolocation:{
 getCurrentPosition:function(ok){ok({coords:{latitude:41.887,longitude:-87.63}});}
}}});
global.XMLHttpRequest=class {
 open(method,url){requests.push(url);}
 send(){
  this.status=200;
  this.responseText=JSON.stringify({current:{time:Math.floor(Date.now()/1000),temperature_2m:17.8,weather_code:2}});
  this.onload();
 }
};
function saveSlots(left,middle,right,city){
 fire('webviewclosed',{response:encodeURIComponent(JSON.stringify({
  LeftTile:{value:String(left)},MiddleTile:{value:String(middle)},RightTile:{value:String(right)},WorldCity:{value:String(city)}
 }))});
}
saveSlots(5,6,7,1);
assert(sent.some(function(packet){return packet.ZoneCity===1;}));
assert(sent.some(function(packet){return packet.Temperature===178;}));
var savedClock=Date.now;
var timeNow=Date.now();
Date.now=function(){return timeNow+1800000;};
fire('appmessage',{payload:{RefreshDesktop:1}});
assert.strictEqual(requests.length,2);
assert(requests[0].includes('latitude=41.89'));
saveSlots(0,1,4,1);
timeNow+=1800000;
fire('appmessage',{payload:{RefreshDesktop:1}});
assert.strictEqual(requests.length,2);
Date.now=savedClock;
console.log('PASS real Clay Save/Cancel/reload, color and time/date options');
