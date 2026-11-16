#include <pebble.h>
#include "config.h"
#include "model.h"
typedef struct {int palette,bg,window,text,accent,time,date;int slots[3];int unit,city;} Settings;
static Settings s={0,0x0055AA,0xAAAAAA,0,0xFFAA00,0,4,{TILE_POWER,TILE_TODAY,TILE_CLOCK},0,1};
#if !defined(PBL_BW)
static const uint32_t PALETTES[6][4]={
 {0x0055AA,0xAAAAAA,0x000000,0xFFAA00},
 {0x000055,0xAAAAAA,0x000000,0xFFAA00},
 {0x005555,0xAAAAAA,0x000000,0xFFFF55},
 {0x550055,0xAAAAAA,0x000000,0xFFAA55},
 {0x555555,0xFFFFFF,0x000000,0xFFFF55},
 {0x000000,0xAAAAAA,0x000000,0xFFFFFF}
};
#endif
static Window *window;static Layer *face;static GBitmap *icons[3];static int width,height,battery;static struct tm current;
static int32_t steps=-1;
static TimeUnits subscribed_units;
static BtWeather weather;
static BtZone zone;
static bool connected;
static time_t next_data_request;
static const char *CITY_LABELS[BT_CITY_COUNT]={
 "UTC","London","Paris","New York","Chicago","Denver","LA","Tokyo","Delhi","Sydney","Auckland"
};
#if BET_FIXTURE
static int fh=BET_HOUR,fm=BET_MINUTE,fs=BET_SECOND,fb=82;static bool proof=false;
static int fixture_steps=8426;
static int32_t fixture_epoch;
#endif
static uint32_t color(int i){
#if defined(PBL_BW)
 static const uint32_t mono[]={0x000000,0xFFFFFF,0x000000,0xFFFFFF};
 return mono[i];
#else
 if(s.palette<6)return PALETTES[s.palette][i];
 return i==0?s.bg:i==1?s.window:i==2?s.text:s.accent;
#endif
}
static GColor display_color(uint32_t ink) {
#if defined(PBL_BW)
 return ((((ink>>16)&255)*299+((ink>>8)&255)*587+(ink&255)*114)>140000)?GColorWhite:GColorBlack;
#else
 return GColorFromHEX(ink);
#endif
}
static GRect rect(Rect r){return GRect(r.x,r.y,r.w,r.h);}
static void fill(GContext *c,GRect r,uint32_t ink){graphics_context_set_fill_color(c,display_color(ink));graphics_fill_rect(c,r,0,GCornerNone);}
static void line(GContext *c,int x,int y,int x2,int y2,uint32_t ink){graphics_context_set_stroke_color(c,display_color(ink));graphics_context_set_stroke_width(c,1);graphics_draw_line(c,GPoint(x,y),GPoint(x2,y2));}
static void label(GContext *c,const char *value,GRect r,uint32_t ink,const char *font){graphics_context_set_text_color(c,display_color(ink));graphics_draw_text(c,value,fonts_get_system_font(font),r,GTextOverflowModeTrailingEllipsis,GTextAlignmentCenter,NULL);}
static uint32_t contrast(uint32_t c){return (((c>>16)&255)*299+((c>>8)&255)*587+(c&255)*114)>140000?0:0xFFFFFF;}
static const uint8_t NUM[10][7]={{14,17,17,17,17,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},{30,17,1,14,1,17,30},{2,6,10,18,31,2,2},{31,16,16,30,1,17,14},{14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},{14,17,17,15,1,1,14}};
/* Bitmap digits preserve native pixel bounds even at 23:59. */
static void digits(GContext *c,const char *value,Rect r,uint32_t ink){int total=0;for(int i=0;value[i];i++)total+=(value[i]==':'?1:5)+1;total--;int offset=0;for(int i=0;value[i];i++){int cols=value[i]==':'?1:5;for(int y=0;y<7;y++)for(int x=0;x<cols;x++){bool on=value[i]==':'?(y==2||y==4):((NUM[value[i]-'0'][y]>>(4-x))&1);if(on){int a=r.x+(offset+x)*r.w/total,b=r.x+(offset+x+1)*r.w/total,t=r.y+y*r.h/7,u=r.y+(y+1)*r.h/7;fill(c,GRect(a,t,b-a,u-t),ink);}}offset+=cols+1;}}
static void compact_date(GContext *c,const char *value,Rect r,uint32_t ink){
 static const uint8_t nums[10][5]={{7,5,5,5,7},{2,6,2,2,7},{7,1,7,4,7},{7,1,7,1,7},{5,5,7,1,1},{7,4,7,1,7},{7,4,7,5,7},{7,1,1,1,1},{7,5,7,5,7},{7,5,7,1,7}};
 int n=strlen(value),left=r.x+(r.w-(n*4-1))/2;
 for(int i=0;i<n;i++)for(int y=0;y<5;y++)for(int x=0;x<3;x++){
  char ch=value[i];bool on=ch>='0'&&ch<='9'?((nums[ch-'0'][y]>>(2-x))&1):ch=='-'?(y==2):ch=='/'?(x==2-y/2):false;
  if(on)fill(c,GRect(left+i*4+x,r.y+4+y,1,1),ink);
 }
}
static void steps_icon(GContext *ctx, Rect icon) {
 int x=icon.x, y=icon.y;
 // BeOS-style rising steps with a dark edge and highlighted treads.
 fill(ctx,GRect(x+5,y+22,23,6),0x000055);
 fill(ctx,GRect(x+12,y+15,16,7),0x000055);
 fill(ctx,GRect(x+19,y+8,9,7),0x000055);
 fill(ctx,GRect(x+5,y+20,7,5),0x55AAFF);
 fill(ctx,GRect(x+12,y+13,7,12),0x55AAFF);
 fill(ctx,GRect(x+19,y+6,7,19),0x55AAFF);
 fill(ctx,GRect(x+5,y+20,7,2),0xAAFFFF);
 fill(ctx,GRect(x+12,y+13,7,2),0xAAFFFF);
 fill(ctx,GRect(x+19,y+6,7,2),0xAAFFFF);
}
static int64_t tile_now(void) {
#if BET_FIXTURE
 if(fixture_epoch>0) return fixture_epoch;
#endif
 return time(NULL);
}
static void tile_circle(GContext *ctx,int x,int y,int radius,uint32_t ink) {
 graphics_context_set_fill_color(ctx,display_color(ink));
 graphics_fill_circle(ctx,GPoint(x,y),radius);
}
static void weather_icon(GContext *ctx, Rect icon, int kind) {
 int x=icon.x, y=icon.y;
 if(kind>=0 && kind<=1) {
  tile_circle(ctx,x+13,y+12,8,0xFFAA00);
  tile_circle(ctx,x+12,y+11,6,0xFFFF55);
  if(kind==0) return;
 }
 const uint32_t cloud_edge=PBL_IF_BW_ELSE(0x000000,0xAAAAAA);
 tile_circle(ctx,x+17,y+15,7,cloud_edge);
 tile_circle(ctx,x+23,y+17,5,cloud_edge);
 tile_circle(ctx,x+11,y+18,5,cloud_edge);
 fill(ctx,GRect(x+10,y+18,15,4),cloud_edge);
 tile_circle(ctx,x+16,y+14,5,0xFFFFFF);
 tile_circle(ctx,x+22,y+16,4,0xFFFFFF);
 tile_circle(ctx,x+11,y+17,4,0xFFFFFF);
 fill(ctx,GRect(x+10,y+17,15,3),0xFFFFFF);
 if(kind==2) {
  line(ctx,x+12,y+24,x+10,y+27,0x55FFFF);
  line(ctx,x+18,y+24,x+16,y+27,0x55FFFF);
  line(ctx,x+24,y+24,x+22,y+27,0x55FFFF);
 } else if(kind==3) {
  fill(ctx,GRect(x+10,y+25,2,2),0xFFFFFF);
  fill(ctx,GRect(x+17,y+26,2,2),0xFFFFFF);
  fill(ctx,GRect(x+23,y+24,2,2),0xFFFFFF);
 }
}
static void phone_icon(GContext *ctx, Rect icon) {
 int x=icon.x,y=icon.y;
 fill(ctx,GRect(x+8,y+4,17,25),0x000055);
 fill(ctx,GRect(x+9,y+5,14,22),0xAAAAAA);
 fill(ctx,GRect(x+11,y+7,10,14),0x005555);
 fill(ctx,GRect(x+12,y+8,8,11),0xAAFFFF);
 fill(ctx,GRect(x+14,y+23,4,2),connected?0x00AA00:0xAA0000);
}
static void world_icon(GContext *ctx, Rect icon) {
 const int x=icon.x,y=icon.y;
 tile_circle(ctx,x+16,y+16,12,PBL_IF_BW_ELSE(0xFFFFFF,0x000055));
 tile_circle(ctx,x+16,y+16,11,0x0055AA);
 tile_circle(ctx,x+15,y+15,10,PBL_IF_BW_ELSE(0x0055AA,0x55AAFF));
 // Europe and Africa, with a pixel of Mediterranean blue between them.
 static const uint8_t coastline[][3]={
  {7,17,3},{8,16,6},{9,16,8},{10,14,10},{11,13,12},
  {12,14,12},{13,17,9},{14,20,6},
  {14,12,6},{15,11,8},{16,11,8},{17,12,8},{18,14,5},
  {19,14,5},{20,15,3},{21,15,3},{22,16,2},{23,16,1}
 };
 for(size_t i=0;i<ARRAY_LENGTH(coastline);i++) {
  const uint8_t *span=coastline[i];
  fill(ctx,GRect(x+span[1],y+span[0],span[2],1),PBL_IF_BW_ELSE(0xFFFFFF,span[0]<18?0x55FF55:0x55AA00));
 }
 // Keep the highlight on the ocean so the coastline stays clean.
 fill(ctx,GRect(x+10,y+9,3,1),0xAAFFFF);
 fill(ctx,GRect(x+9,y+10,2,1),0xAAFFFF);
}
static void draw_tile(GContext *ctx,const Layout *layout,int slot,uint32_t ink) {
 static const char *names[]={"Power","Today","Clock","Steps"};
 const int tile=s.slots[slot];
 int64_t now=tile_now();
 bool have_weather=tile==TILE_WEATHER && bt_weather_fresh(&weather,now);
 if(tile==TILE_EMPTY) return;
 if(tile==TILE_STEPS) steps_icon(ctx,layout->icons[slot]);
 else if(tile==TILE_WEATHER) weather_icon(ctx,layout->icons[slot],have_weather?weather.kind:-1);
 else if(tile==TILE_PHONE) phone_icon(ctx,layout->icons[slot]);
 else if(tile==TILE_WORLD) world_icon(ctx,layout->icons[slot]);
 else if(icons[tile]) {
  graphics_context_set_compositing_mode(ctx,GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx,icons[tile],rect(layout->icons[slot]));
 }
 const char *name=tile==TILE_WORLD?CITY_LABELS[s.city]:
                  tile==TILE_WEATHER?"Weather":tile==TILE_PHONE?"Phone":names[tile];
 label(ctx,name,rect(layout->labels[slot]),ink,width<=180?FONT_KEY_GOTHIC_14:FONT_KEY_GOTHIC_14_BOLD);
 char value[16]="--";
 switch(tile) {
  case TILE_POWER: snprintf(value,sizeof(value),"%d%%",battery); break;
  case TILE_TODAY: bt_date(current.tm_year+1900,current.tm_mon+1,current.tm_mday,s.date,value,sizeof(value)); break;
  case TILE_CLOCK: snprintf(value,sizeof(value),"%02d SEC",current.tm_sec); break;
  case TILE_STEPS:
   if(steps>=0) snprintf(value,sizeof(value),"%ld",(long)steps);
   break;
  case TILE_WEATHER:
   if(have_weather) {
    int tenths=weather.temperature;
    if(!s.unit) tenths=tenths*9/5+320;
    int rounded=(tenths+(tenths>=0?5:-5))/10;
    snprintf(value,sizeof(value),"%d°%c",rounded,s.unit?'C':'F');
   }
   break;
  case TILE_PHONE: snprintf(value,sizeof(value),"%s",connected?"LINKED":"OFFLINE"); break;
  case TILE_WORLD:
   if(s.city==0 || bt_zone_fresh(&zone,s.city,now)) {
    const BtZone utc={0};
    bt_zone_time(s.city==0?&utc:&zone,now,bt_24(s.time,clock_is_24h_style()),value,sizeof(value));
   }
   break;
 }
 if(tile==TILE_TODAY && s.date>=1 && s.date<=3) compact_date(ctx,value,layout->values[slot],ink);
 else label(ctx,value,rect(layout->values[slot]),ink,FONT_KEY_GOTHIC_14);
}
static void refresh_steps(void) {
 if(!bt_has_tile(s.slots,TILE_STEPS)) return;
 steps=-1;
#if BET_FIXTURE
 steps=fixture_steps;
#elif defined(PBL_HEALTH)
 time_t end=time(NULL);
 time_t start=time_start_of_today();
 if(health_service_metric_accessible(HealthMetricStepCount,start,end)&HealthServiceAccessibilityMaskAvailable)
  steps=health_service_sum_today(HealthMetricStepCount);
#endif
}
static void draw(Layer *layer,GContext *c){
 graphics_context_set_antialiased(c,false);fill(c,layer_get_bounds(layer),color(0));Layout l;bt_layout(width,height,&l);Rect p=l.panel,t=l.tab;
 fill(c,GRect(p.x+3,p.y+3,p.w,p.h),0x000055);fill(c,rect(p),0x555555);line(c,p.x,p.y,p.x+p.w-1,p.y,0xFFFFFF);line(c,p.x,p.y,p.x,p.y+p.h-1,0xFFFFFF);fill(c,GRect(p.x+3,p.y+3,p.w-6,p.h-6),color(1));
 uint32_t ink=color(2);fill(c,rect(t),0);fill(c,GRect(t.x+1,t.y+1,t.w-2,t.h-2),color(3));line(c,t.x+2,t.y+1,t.x+t.w-3,t.y+1,0xFFFFFF);fill(c,GRect(t.x+7,t.y+7,8,8),0);fill(c,GRect(t.x+8,t.y+8,6,6),color(1));label(c,"Clock",GRect(t.x+20,t.y-3,t.w-24,22),contrast(color(3)),FONT_KEY_GOTHIC_18_BOLD);
 char value[12];bt_time(current.tm_hour,current.tm_min,s.time,clock_is_24h_style(),value,sizeof(value));digits(c,value,l.time,ink);
 if(!bt_24(s.time,clock_is_24h_style()))label(c,current.tm_hour>=12?"PM":"AM",rect(l.ampm),ink,FONT_KEY_GOTHIC_14_BOLD);
 line(c,p.x+8,l.brand.y-3,p.x+p.w-9,l.brand.y-3,0x555555);label(c,"BeTime",rect(l.brand),ink,FONT_KEY_GOTHIC_14);
 uint32_t desk_ink=contrast(color(0));
 for(int i=0;i<3;i++) draw_tile(c,&l,i,desk_ink);
#if BET_FIXTURE
 if(proof){const int y=width==144?7:19;fill(c,GRect(width/2-4,y,2,2),PBL_IF_BW_ELSE(0xFFFFFF,0xFF00FF));fill(c,GRect(width/2+2,y,2,2),bt_bounds(width,height,&l)?PBL_IF_BW_ELSE(0xFFFFFF,0x00FF00):0xFF0000);fill(c,GRect(width/2+8,y,2,2),(subscribed_units&SECOND_UNIT)?PBL_IF_BW_ELSE(0x000000,0xFFAA00):0x55FFFF);}
#endif
}
static void request_desktop_data(void) {
 if(!connected || (!bt_has_tile(s.slots,TILE_WEATHER) &&
                   !(s.city!=0 && bt_has_tile(s.slots,TILE_WORLD)))) return;
 time_t now=time(NULL);
 if(now<next_data_request) return;
 DictionaryIterator *out;
 if(app_message_outbox_begin(&out)!=APP_MSG_OK) return;
 if(dict_write_uint8(out,MESSAGE_KEY_RefreshDesktop,1)==DICT_OK &&
    app_message_outbox_send()==APP_MSG_OK) next_data_request=now+30*60;
}
static void tick(struct tm *now,TimeUnits changed){current=*now;
#if BET_FIXTURE
 current.tm_hour=fh;current.tm_min=fm;current.tm_sec=fs;current.tm_year=126;current.tm_mon=10;current.tm_mday=8;battery=fb;
#endif
 if(changed&MINUTE_UNIT) {
  refresh_steps();
  request_desktop_data();
 }
 layer_mark_dirty(face);}
static void subscribe_ticks(void) {
 TimeUnits units=bt_has_tile(s.slots,TILE_CLOCK)?(SECOND_UNIT|MINUTE_UNIT):MINUTE_UNIT;
 if(units==subscribed_units) return;
 tick_timer_service_unsubscribe();
 tick_timer_service_subscribe(units,tick);
 subscribed_units=units;
}
static void battery_changed(BatteryChargeState state){battery=state.charge_percent;
#if BET_FIXTURE
 battery=fb;
#endif
 if(bt_has_tile(s.slots,TILE_POWER)) layer_mark_dirty(face);}
enum { P_LEFT_TILE=8, P_MIDDLE_TILE, P_RIGHT_TILE, P_UNIT, P_CITY, P_WEATHER, P_ZONE };
static void connection_changed(bool available) {
 connected=available;
 if(available) next_data_request=0;
 if(bt_has_tile(s.slots,TILE_PHONE)) layer_mark_dirty(face);
}
static bool read_number(DictionaryIterator *it,uint32_t key,int32_t *value) {
 Tuple *item=dict_find(it,key);
 if(!item || (item->type!=TUPLE_INT && item->type!=TUPLE_UINT)) return false;
 switch(item->length) {
  case 1: *value=item->type==TUPLE_INT?item->value->int8:item->value->uint8; return true;
  case 2: *value=item->type==TUPLE_INT?item->value->int16:item->value->uint16; return true;
  case 4: *value=item->value->int32; return true;
 }
 return false;
}
static void receive_tiles(DictionaryIterator *it) {
 BtWeather incoming_weather;
 if(read_number(it,MESSAGE_KEY_WeatherAt,&incoming_weather.at) &&
    read_number(it,MESSAGE_KEY_Temperature,&incoming_weather.temperature) &&
    read_number(it,MESSAGE_KEY_WeatherKind,&incoming_weather.kind) &&
    incoming_weather.at>=weather.at && bt_weather_fresh(&incoming_weather,tile_now())) {
  weather=incoming_weather;
  persist_write_data(P_WEATHER,&weather,sizeof(weather));
 }
 BtZone incoming_zone;
 if(read_number(it,MESSAGE_KEY_ZoneCity,&incoming_zone.city) &&
    read_number(it,MESSAGE_KEY_ZoneOffset,&incoming_zone.offset) &&
    read_number(it,MESSAGE_KEY_ZoneNextOffset,&incoming_zone.next_offset) &&
    read_number(it,MESSAGE_KEY_ZoneChangeAt,&incoming_zone.change_at) &&
    read_number(it,MESSAGE_KEY_ZoneUntil,&incoming_zone.until) &&
    bt_zone_fresh(&incoming_zone,s.city,tile_now()) &&
    (zone.city!=s.city || incoming_zone.until>=zone.until)) {
  zone=incoming_zone;
  persist_write_data(P_ZONE,&zone,sizeof(zone));
 }
}
static void setting(DictionaryIterator *it,int message,int key,int max,int *value){Tuple *t=dict_find(it,message);if(!t||(t->type!=TUPLE_INT&&t->type!=TUPLE_UINT))return;int v=t->value->int32;if(v<0||v>max)return;*value=v;persist_write_int(key,v);}
static void inbox(DictionaryIterator *it,void *context){(void)context;setting(it,MESSAGE_KEY_Palette,1,6,&s.palette);setting(it,MESSAGE_KEY_Background,2,0xFFFFFF,&s.bg);setting(it,MESSAGE_KEY_WindowColor,3,0xFFFFFF,&s.window);setting(it,MESSAGE_KEY_TextColor,4,0xFFFFFF,&s.text);setting(it,MESSAGE_KEY_AccentColor,5,0xFFFFFF,&s.accent);setting(it,MESSAGE_KEY_TimeFormat,6,2,&s.time);setting(it,MESSAGE_KEY_DateFormat,7,4,&s.date);
 setting(it,MESSAGE_KEY_LeftTile,P_LEFT_TILE,TILE_WORLD,&s.slots[0]);
 setting(it,MESSAGE_KEY_MiddleTile,P_MIDDLE_TILE,TILE_WORLD,&s.slots[1]);
 setting(it,MESSAGE_KEY_RightTile,P_RIGHT_TILE,TILE_WORLD,&s.slots[2]);
 setting(it,MESSAGE_KEY_TemperatureUnit,P_UNIT,1,&s.unit);
 setting(it,MESSAGE_KEY_WorldCity,P_CITY,BT_CITY_COUNT-1,&s.city);
#if BET_FIXTURE
 int *fields[]={&fh,&fm,&fs,&fb};int max[]={23,59,59,100};for(int i=0;i<4;i++){Tuple *t=dict_find(it,11000+i);if(t&&t->type==TUPLE_INT&&t->value->int32>=0&&t->value->int32<=max[i])*fields[i]=t->value->int32;}Tuple *step=dict_find(it,11004);if(step&&step->type==TUPLE_INT&&step->value->int32>=-1&&step->value->int32<=999999)fixture_steps=step->value->int32;Tuple *p=dict_find(it,11022);if(p&&p->type==TUPLE_INT)proof=p->value->int32==1;
 int32_t fixture_connected;
 read_number(it,11005,&fixture_epoch);
 if(read_number(it,11006,&fixture_connected)) connected=fixture_connected!=0;
#endif
 receive_tiles(it);
 window_set_background_color(window,GColorFromHEX(color(0)));
 subscribe_ticks();
 // Settings changes refresh data on the phone after this message is acknowledged.
 time_t now=time(NULL);tick(localtime(&now),0);refresh_steps();
}
static void restore(int key,int max,int *v){if(persist_exists(key)){int n=persist_read_int(key);if(n>=0&&n<=max)*v=n;}}
static void init(void){restore(1,6,&s.palette);restore(2,0xFFFFFF,&s.bg);restore(3,0xFFFFFF,&s.window);restore(4,0xFFFFFF,&s.text);restore(5,0xFFFFFF,&s.accent);restore(6,2,&s.time);restore(7,4,&s.date);
 for(int slot=0;slot<3;slot++) restore(P_LEFT_TILE+slot,TILE_WORLD,&s.slots[slot]);
 restore(P_UNIT,1,&s.unit);restore(P_CITY,BT_CITY_COUNT-1,&s.city);
 if(persist_read_data(P_WEATHER,&weather,sizeof(weather))!=sizeof(weather)) weather=(BtWeather){0};
 if(persist_read_data(P_ZONE,&zone,sizeof(zone))!=sizeof(zone)) zone=(BtZone){0};
 icons[0]=gbitmap_create_with_resource(RESOURCE_ID_ICON_POWER);icons[1]=gbitmap_create_with_resource(RESOURCE_ID_ICON_CALENDAR);icons[2]=gbitmap_create_with_resource(RESOURCE_ID_ICON_CLOCK);
 window=window_create();window_set_background_color(window,GColorFromHEX(color(0)));Layer *root=window_get_root_layer(window);GRect b=layer_get_bounds(root);width=b.size.w;height=b.size.h;face=layer_create(b);layer_set_update_proc(face,draw);layer_add_child(root,face);window_stack_push(window,true);battery=battery_state_service_peek().charge_percent;time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);subscribe_ticks();battery_state_service_subscribe(battery_changed);
 connected=connection_service_peek_pebble_app_connection();
 next_data_request=time(NULL)+30*60;
 connection_service_subscribe((ConnectionHandlers){.pebble_app_connection_handler=connection_changed});
 app_message_register_inbox_received(inbox);app_message_open(256,64);}
static void deinit(void){tick_timer_service_unsubscribe();battery_state_service_unsubscribe();connection_service_unsubscribe();app_message_deregister_callbacks();for(int i=0;i<3;i++)gbitmap_destroy(icons[i]);layer_destroy(face);window_destroy(window);}
int main(void){init();app_event_loop();deinit();}
