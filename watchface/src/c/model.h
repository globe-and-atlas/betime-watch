#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct {int x,y,w,h;} Rect;
typedef struct {Rect panel,tab,icons[3],labels[3],values[3],time,ampm,brand;} Layout;
bool bt_24(int mode,bool system);
void bt_time(int hour,int minute,int mode,bool system,char *out,size_t size);
void bt_date(int year,int month,int day,int format,char *out,size_t size);
void bt_layout(int width,int height,Layout *out);
bool bt_bounds(int width,int height,const Layout *l);

typedef enum { TILE_POWER, TILE_TODAY, TILE_CLOCK, TILE_STEPS, TILE_EMPTY, TILE_WEATHER, TILE_PHONE, TILE_WORLD } Tile;
bool bt_has_tile(const int slots[3], Tile tile);

#define BT_CITY_COUNT 11
typedef struct {int32_t at, temperature, kind;} BtWeather;
typedef struct {int32_t city, offset, next_offset, change_at, until;} BtZone;
bool bt_weather_fresh(const BtWeather *weather, int64_t now);
bool bt_zone_fresh(const BtZone *zone, int city, int64_t now);
void bt_zone_time(const BtZone *zone, int64_t now, bool use24, char *out, size_t size);
