#include "model.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){
 const int sizes[][2]={{200,228},{260,260},{144,168},{180,180}};
 for(size_t target=0;target<sizeof(sizes)/sizeof(sizes[0]);target++){int w=sizes[target][0],h=sizes[target][1];Layout l;bt_layout(w,h,&l);assert(bt_bounds(w,h,&l));}
 assert(!bt_24(1,true));assert(bt_24(2,false));assert(bt_24(0,true));assert(!bt_24(0,false));
 for(int t=0;t<86400;t++)for(int mode=0;mode<3;mode++)for(int system=0;system<2;system++){char out[12],want[12];int h=t/3600,m=t/60%60;int expected=(mode==2||(mode==0&&system))?h:(h%12?h%12:12);snprintf(want,sizeof(want),"%02d:%02d",expected,m);bt_time(h,m,mode,system,out,sizeof(out));assert(!strcmp(out,want));}
 int slots[]={TILE_POWER,TILE_TODAY,TILE_CLOCK};
 assert(bt_has_tile(slots,TILE_CLOCK));slots[2]=TILE_STEPS;assert(!bt_has_tile(slots,TILE_CLOCK));assert(bt_has_tile(slots,TILE_STEPS));slots[0]=TILE_CLOCK;assert(bt_has_tile(slots,TILE_CLOCK));
 BtWeather weather={10000,178,1};
 assert(bt_weather_fresh(&weather,15400));assert(!bt_weather_fresh(&weather,15401));
 BtZone zone={1,0,60,86400,90000};char local[16];
 assert(bt_zone_fresh(&zone,1,86340));assert(!bt_zone_fresh(&zone,2,86340));
 bt_zone_time(&zone,86340,true,local,sizeof(local));assert(!strcmp(local,"23:59"));
 bt_zone_time(&zone,86400,false,local,sizeof(local));assert(!strcmp(local,"1:00 AM"));
 char out[16];bt_date(2026,11,8,0,out,sizeof(out));assert(!strcmp(out,"--"));bt_date(2026,11,8,1,out,sizeof(out));assert(!strcmp(out,"2026-11-08"));bt_date(2026,11,8,2,out,sizeof(out));assert(!strcmp(out,"11/08/2026"));bt_date(2026,11,8,3,out,sizeof(out));assert(!strcmp(out,"08/11/2026"));bt_date(2026,11,8,4,out,sizeof(out));assert(!strcmp(out,"NOV 08"));bt_date(2028,2,29,4,out,sizeof(out));assert(!strcmp(out,"FEB 29"));puts("PASS full-day time modes, dates, four screen geometries");return 0;
}
