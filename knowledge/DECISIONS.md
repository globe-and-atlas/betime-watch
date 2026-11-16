---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-08T17:11:48-06:00"
---
# Decisions

## 2026-11-08
Use native C for rendering, pure C formatting/layout for host verification, Clay 1.1.0 for settings. No interactive desktop: icons display battery, date, seconds. Six desktop palettes preserve the yellow short Clock tab; custom colors override desktop/window/tab/text. Date defaults to the approved short month/day format; Off and ISO/US/EU available. Icon artwork is retained, resized to 32px and quantized to Pebble's 64 colors. Preserve original assets and notices. Only SECOND_UNIT updates, no animation timer or external service.

## 2026-11-10 — Desktop tile choices

Persist three independent tile selections and append the AppMessage IDs. Keep Battery, Date, and Seconds as the defaults for older settings. Steps reads Pebble Health on the minute only while a Steps tile is visible. Seconds enables second ticks in addition to minute ticks. Battery callbacks redraw only when a Power tile is visible.

## 2026-11-10 — Weather, connection, and second time zone

Request weather and named-city updates from the existing minute callback at most every 30 minutes while connected. This avoids relying on phone timers that iOS may suspend in the background. Weather and time-zone packets are cached as complete records. Expired weather or a named city without a valid offset displays --. Use the phone's time-zone database to send the offset and the transition within a 36-hour window. The watch computes UTC locally and applies a cached offset change at its scheduled minute.
