'use strict';

var cities = [
  ['UTC', 'UTC'], ['London', 'Europe/London'], ['Paris', 'Europe/Paris'],
  ['New York', 'America/New_York'], ['Chicago', 'America/Chicago'],
  ['Denver', 'America/Denver'], ['Los Angeles', 'America/Los_Angeles'],
  ['Tokyo', 'Asia/Tokyo'], ['Delhi', 'Asia/Kolkata'],
  ['Sydney', 'Australia/Sydney'], ['Auckland', 'Pacific/Auckland']
];

function weatherKind(code) {
  if (!Number.isInteger(code)) return null;
  if (code === 0) return 0;
  if ([1, 2, 3, 45, 48].indexOf(code) !== -1) return 1;
  if ([51, 53, 55, 56, 57, 61, 63, 65, 66, 67, 80, 81, 82, 95, 96, 99].indexOf(code) !== -1) return 2;
  if ([71, 73, 75, 77, 85, 86].indexOf(code) !== -1) return 3;
  return null;
}

function weatherMessage(payload, now) {
  var current = payload && payload.current;
  if (!current || !Number.isInteger(current.time) || Math.abs(now - current.time) > 1800) return null;
  var temp = current.temperature_2m;
  var kind = weatherKind(current.weather_code);
  if (typeof temp !== 'number' || !isFinite(temp) || temp < -100 || temp > 70 || kind === null) return null;
  return { WeatherAt: current.time, Temperature: Math.round(temp * 10), WeatherKind: kind };
}

function weatherUrl(latitude, longitude) {
  return 'https://api.open-meteo.com/v1/forecast?latitude=' + latitude.toFixed(2) +
    '&longitude=' + longitude.toFixed(2) + '&current=temperature_2m,weather_code&timeformat=unixtime';
}

function worldMessage(city, now) {
  var epoch = Math.floor(now / 60) * 60;
  var deadline = epoch + 36 * 3600;
  var offset = 0, next = 0, transition = deadline;
  try {
    if (city !== 0) {
      var formatter = new Intl.DateTimeFormat('en-GB', {
        timeZone: cities[city][1], year: 'numeric', month: '2-digit', day: '2-digit',
        hour: '2-digit', minute: '2-digit', hourCycle: 'h23'
      });
      function offsetAt(stamp) {
        var parts = {};
        formatter.formatToParts(new Date(stamp * 1000)).forEach(function (part) {
          parts[part.type] = Number(part.value);
        });
        var local = Date.UTC(parts.year, parts.month - 1, parts.day, parts.hour % 24, parts.minute);
        var minutes = (local / 1000 - stamp) / 60;
        if (!Number.isInteger(minutes) || minutes < -720 || minutes > 840) throw new Error('Invalid UTC offset');
        return minutes;
      }
      offset = offsetAt(epoch);
      next = offsetAt(deadline);
      if (next !== offset) {
        var low = epoch, high = deadline;
        while (high - low > 60) {
          var middle = Math.floor((low + high) / 120) * 60;
          if (offsetAt(middle) === offset) low = middle;
          else high = middle;
        }
        transition = high;
      }
    }
  } catch (error) {
    return null;
  }
  return { ZoneCity: city, ZoneOffset: offset, ZoneNextOffset: next, ZoneChangeAt: transition, ZoneUntil: deadline };
}

module.exports = { cities: cities, weatherMessage: weatherMessage, weatherUrl: weatherUrl, worldMessage: worldMessage };
