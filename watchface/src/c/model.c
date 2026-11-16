#include "model.h"
#include <stdio.h>
bool bt_24(int mode,bool system){return mode==2||(mode==0&&system);}
void bt_time(int hour,int minute,int mode,bool system,char *out,size_t size){int h=bt_24(mode,system)?hour:hour%12;if(!bt_24(mode,system)&&!h)h=12;snprintf(out,size,"%02d:%02d",h,minute);}
void bt_date(int year,int month,int day,int format,char *out,size_t size){
 static const char *months[]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
 if(format==0)snprintf(out,size,"--");
 else if(format==1)snprintf(out,size,"%04d-%02d-%02d",year,month,day);
 else if(format==2)snprintf(out,size,"%02d/%02d/%04d",month,day,year);
 else if(format==3)snprintf(out,size,"%02d/%02d/%04d",day,month,year);
 else snprintf(out,size,"%s %02d",months[month>=1&&month<=12?month-1:0],day);
}
void bt_layout(int width,int height,Layout *l){
 if(width==144 && height==168) {
  *l=(Layout){.panel={7,34,127,76},.tab={11,16,73,20},.time={16,54,87,28},.ampm={106,60,23,18},.brand={15,91,111,16}};
  for(int i=0;i<3;i++){
   int center=25+47*i;
   l->icons[i]=(Rect){center-16,112,32,32};
   l->labels[i]=(Rect){center-23,139,46,14};
   l->values[i]=(Rect){center-23,152,46,15};
  }
  return;
 }
 if(width==180 && height==180) {
  *l=(Layout){.panel={23,43,134,53},.tab={31,25,73,20},.time={32,53,94,24},.ampm={129,56,22,18},.brand={31,81,118,14}};
  for(int i=0;i<3;i++){
   const int center=46+44*i;
   // Raise the outer tiles to follow the circular lower edge.
   const int y=i==1?105:97;
   l->icons[i]=(Rect){center-16,y,32,32};
   l->labels[i]=(Rect){center-23,y+26,46,14};
   l->values[i]=(Rect){center-23,y+38,46,14};
  }
  return;
 }
 bool round=width==height;int x=round?33:9,y=round?60:48,w=round?194:182;
 *l=(Layout){.panel={x,y,w,100},.tab={x+4,y-18,92,20},.time={width/2-77,y+25,128,35},.ampm={x+w-27,y+33,22,18},.brand={x+8,y+76,w-16,17}};
 int centers[3]={round?72:37,round?130:100,round?188:163};int iy=round?163:158;
 for(int i=0;i<3;i++){l->icons[i]=(Rect){centers[i]-16,iy,32,32};l->labels[i]=(Rect){centers[i]-28,iy+32,56,16};l->values[i]=(Rect){centers[i]-27,iy+49,54,14};}
}
static bool inside(int w,int h,Rect r){
 if(r.w<=0||r.h<=0||r.x<0||r.y<0||r.x+r.w>w||r.y+r.h>h)return false;
 if(w==h) {
  // Work in half-pixel units to keep the circular check exact without libm.
  const int diameter=w-2;
  const int xs[]={2*r.x-w+1,2*(r.x+r.w-1)-w+1};
  const int ys[]={2*r.y-h+1,2*(r.y+r.h-1)-h+1};
  for(int i=0;i<2;i++) for(int j=0;j<2;j++)
   if(xs[i]*xs[i]+ys[j]*ys[j]>diameter*diameter) return false;
 }
 return true;
}
bool bt_bounds(int w,int h,const Layout *l){
 Rect main[]={l->panel,l->tab,l->time,l->ampm,l->brand};for(int i=0;i<5;i++)if(!inside(w,h,main[i]))return false;
 if(l->time.x+l->time.w>l->ampm.x)return false;
 for(int i=0;i<3;i++){if(!inside(w,h,l->icons[i])||!inside(w,h,l->labels[i])||!inside(w,h,l->values[i]))return false;if(l->panel.y+l->panel.h>l->icons[i].y||l->values[i].y+l->values[i].h>h-(w==144?1:6))return false;}
 return true;
}

bool bt_has_tile(const int slots[3], Tile tile) {
 for(int i=0;i<3;i++) if(slots[i]==(int)tile) return true;
 return false;
}

bool bt_weather_fresh(const BtWeather *weather, int64_t now) {
 return weather->at > 0 && now >= (int64_t)weather->at-300 && now-(int64_t)weather->at <= 90*60 &&
        weather->temperature >= -1000 && weather->temperature <= 700 &&
        weather->kind >= 0 && weather->kind <= 3;
}
bool bt_zone_fresh(const BtZone *zone, int city, int64_t now) {
 return city >= 0 && city < BT_CITY_COUNT && zone->city == city &&
        zone->offset >= -720 && zone->offset <= 840 &&
        zone->next_offset >= -720 && zone->next_offset <= 840 &&
        zone->until > now && zone->until-now <= 36*3600+300 &&
        zone->change_at > 0 && zone->change_at <= zone->until;
}
void bt_zone_time(const BtZone *zone, int64_t now, bool use24, char *out, size_t size) {
 int offset = now >= zone->change_at ? zone->next_offset : zone->offset;
 int minute = (int)((now/60+offset)%1440);
 if(minute<0) minute+=1440;
 int hour=minute/60;
 if(use24) snprintf(out,size,"%02d:%02d",hour,minute%60);
 else snprintf(out,size,"%d:%02d %s",hour%12?hour%12:12,minute%60,hour>=12?"PM":"AM");
}
