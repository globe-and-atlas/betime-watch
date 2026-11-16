---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-08T17:18:18-06:00"
---
# BeTime

A tiny BeOS-style desktop on your wrist: a short yellow Clock tab, beveled window, and actual Haiku ecosystem icons. The default desktop shows Power, Today, and Clock. Choose battery, date, seconds, daily steps, weather, phone connection, a world clock, or an empty slot independently for each position in Pebble settings.

Targets include Pebble Time 2 (200×228), Round 2 (260×260), Time Round (180×180), and the 144×168 Pebble 2, Pebble 2 Duo, Time, and Time Steel. Color watches offer six palettes, custom desktop/window/text/tab colors, and original icon colors. Pebble 2 and Pebble 2 Duo use a fixed monochrome palette. Choose watch-default, 12-hour, or 24-hour time. Date formats include Off, ISO, US, EU, and short month-day.

Settings use Clay1.1.0 through the Pebble app. Time and battery work offline. Steps reads Pebble Health on the watch and displays -- when data is unavailable. The clock uses minute ticks whenever no slot shows seconds. A Steps tile refreshes with the minute tick. Weather uses approximate phone location, Open-Meteo, and Fahrenheit or Celsius. Weather expires after 90 minutes. With Weather or a named World Clock city selected, the connected watch requests fresh data every 30 minutes using its existing minute tick. The Phone tile reacts to the Pebble app connection. World Clock offers 11 time zones and uses the phone’s daylight-saving rules. UTC works without a phone. Named cities use cached offsets for up to 36 hours.

## Build and verify

Install watchface dependencies with npm ci --ignore-scripts. Run python3 execution/build.py --host-only for formatting/layout and actual Clay tests. Run python3 execution/build.py for clean production; python3 execution/verify.py validates identity, binary bytes, bundled notices, fixture exclusion. The local update builds as dist/BeTime-0.4.1.pbw.

For native fixtures, run build_fixture.py then capture_native.py --platform emery or gabbro --reset-emulator, after checking exclusive shared emulator ownership. Reset backs up flash. Never publish fixtures; rebuild clean production after captures.

See THIRD_PARTY_NOTICES.txt for Haiku/Haiku Calendar MIT attribution. Originals are in media/source-icons/. The three sourced icons are resized to 32 px and mapped to the Pebble palette. The Steps, Weather, Phone, and World Clock icons are original artwork.

BeTime 0.4.1 is available on the [Pebble Appstore](https://apps.repebble.com/f77f655a2c9f4b72a2f39e58). The Time 2 and Round 2 galleries open with a seven-frame palette GIF followed by four native screenshots. The Time Round and three compact galleries show native Weather, Steps, and London stills. Physical-watch battery endurance remains unmeasured.
