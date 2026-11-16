var Clay=require('@rebble/clay');
var data=require('./desktop-data');
var defaults={Palette:0,Background:0x0055AA,WindowColor:0xAAAAAA,TextColor:0,AccentColor:0xFFAA00,TimeFormat:0,DateFormat:4,LeftTile:0,MiddleTile:1,RightTile:2,TemperatureUnit:0,WorldCity:1};
function integer(v,f,max){if(v===undefined||v===null||v===''||typeof v==='boolean')return f;var n=Number(v);return isFinite(n)&&Math.floor(n)===n&&n>=0&&n<=max?n:f;}
function normalize(raw){raw=raw||{};var out={};Object.keys(defaults).forEach(function(k){out[k]=integer(raw[k],defaults[k],k==='Palette'?6:k==='TimeFormat'?2:k==='DateFormat'?4:/Tile$/.test(k)?7:k==='TemperatureUnit'?1:k==='WorldCity'?data.cities.length-1:0xFFFFFF);});return out;}
var tileOptions=[
 {label:'Power · battery',value:'0'},{label:'Today · date',value:'1'},
 {label:'Clock · seconds',value:'2'},{label:'Steps · daily count',value:'3'},
 {label:'Empty',value:'4'},{label:'Weather · temperature',value:'5'},
 {label:'Phone · connection',value:'6'},{label:'World Clock · second time zone',value:'7'}
];
var clay=new Clay([
 {type:'heading',defaultValue:'BeTime'},
 {type:'text',defaultValue:'A tiny BeOS-style desktop. Choose what each desktop icon shows. Artwork from Haiku and Haiku Calendar, used under MIT licenses.'},
 {type:'select',messageKey:'TimeFormat',label:'Time format',defaultValue:'0',options:[{label:'Watch default',value:'0'},{label:'12-hour',value:'1'},{label:'24-hour',value:'2'}]},
 {type:'select',messageKey:'DateFormat',label:'Today date',defaultValue:'4',options:[{label:'Off',value:'0'},{label:'YYYY-MM-DD',value:'1'},{label:'MM/DD/YYYY',value:'2'},{label:'DD/MM/YYYY',value:'3'},{label:'Short month and day',value:'4'}]},
 {type:'heading',defaultValue:'Desktop icons'},
 {type:'select',messageKey:'LeftTile',label:'Left icon',defaultValue:'0',options:tileOptions},
 {type:'select',messageKey:'MiddleTile',label:'Middle icon',defaultValue:'1',options:tileOptions},
 {type:'select',messageKey:'RightTile',label:'Right icon',defaultValue:'2',options:tileOptions},
 {type:'text',defaultValue:'Steps uses Pebble Health. With no Clock icon, the watchface updates on the minute.'},
 {type:'select',messageKey:'TemperatureUnit',label:'Weather units',defaultValue:'0',
  options:[{label:'Fahrenheit',value:'0'},{label:'Celsius',value:'1'}]},
 {type:'select',messageKey:'WorldCity',label:'World Clock city',defaultValue:'1',
  options:data.cities.map(function(city,index){return {label:city[0],value:String(index)};})},
 {type:'text',defaultValue:'Weather uses approximate phone location and Open-Meteo. World Clock uses your phone’s time-zone rules.'},
 {type:'heading',defaultValue:'Colors'},
 {type:'select',messageKey:'Palette',label:'Desktop palette',defaultValue:'0',options:[{label:'Classic blue',value:'0'},{label:'Midnight',value:'1'},{label:'Teal',value:'2'},{label:'Plum',value:'3'},{label:'Graphite',value:'4'},{label:'Monochrome desktop',value:'5'},{label:'Custom colors',value:'6'}]},
 {type:'text',defaultValue:'Select Custom colors to use the four colors below. Icon artwork keeps its original colors.'},
 {type:'color',messageKey:'Background',label:'Desktop',defaultValue:'0055AA',sunlight:false,allowGray:true},
 {type:'color',messageKey:'WindowColor',label:'Clock window',defaultValue:'AAAAAA',sunlight:false,allowGray:true},
 {type:'color',messageKey:'TextColor',label:'Clock text',defaultValue:'000000',sunlight:false,allowGray:true},
 {type:'color',messageKey:'AccentColor',label:'Clock tab',defaultValue:'FFAA00',sunlight:false,allowGray:true},
 {type:'submit',defaultValue:'Save to watch'}
],null,{autoHandleEvents:false});
var active=defaults;
var fetchId=0;
var lastWeatherAttempt=null;
var refreshPeriod=30*60*1000;

function hasTile(tile) {
 return active.LeftTile===tile || active.MiddleTile===tile || active.RightTile===tile;
}

function refreshWeather() {
 if(!hasTile(5) || typeof navigator==='undefined' || !navigator.geolocation) return;
 var now=Date.now();
 if(lastWeatherAttempt!==null && now-lastWeatherAttempt<60000) return;
 lastWeatherAttempt=now;
 var id=++fetchId;
 function ignore(){}
 navigator.geolocation.getCurrentPosition(function(position) {
  if(id!==fetchId || !hasTile(5)) return;
  var request=new XMLHttpRequest();
  request.open('GET',data.weatherUrl(position.coords.latitude,position.coords.longitude),true);
  request.timeout=12000;
  request.onload=function(){
   if(id!==fetchId || !hasTile(5) || request.status!==200) return;
   try {
    var packet=data.weatherMessage(JSON.parse(request.responseText),Math.floor(Date.now()/1000));
    if(packet) Pebble.sendAppMessage(packet);
   } catch(error) {console.log('BeTime weather response could not be read');}
  };
  request.onerror=request.ontimeout=request.onabort=ignore;
  request.send();
 },ignore,{timeout:12000,maximumAge:refreshPeriod,enableHighAccuracy:false});
}

function refresh() {
 if(hasTile(7) && active.WorldCity!==0) {
  var packet=data.worldMessage(active.WorldCity,Math.floor(Date.now()/1000));
  if(packet) Pebble.sendAppMessage(packet);
 }
 refreshWeather();
}

function sendSaved() {
 var raw=localStorage.getItem('clay-settings');
 if(!raw) return;
 try {
  active=normalize(JSON.parse(raw));
  Pebble.sendAppMessage(active,refresh);
 } catch(error) {console.log('BeTime settings could not be loaded');}
}
Pebble.addEventListener('ready',sendSaved);
Pebble.addEventListener('appmessage',function(event){if(event.payload && event.payload.RefreshDesktop)refresh();});Pebble.addEventListener('showConfiguration',function(){Pebble.openURL(clay.generateUrl());});Pebble.addEventListener('webviewclosed',function(event){if(!event||!event.response||event.response==='CANCELLED')return;try{clay.getSettings(event.response,false);sendSaved();}catch(error){console.log('BeTime settings were not saved');}});
module.exports={normalize:normalize,defaults:defaults};
