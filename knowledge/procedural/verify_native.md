---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-08T17:24:01-06:00"
---
# Verify BeTime natively

Run python3 -m pytest -q and python3 execution/build_fixture.py. Drive only one emulator at a time. Confirm ownership before using capture_native.py --platform emery or gabbro --reset-emulator. Reset backs up SDK flash; never hard-kill QEMU. The fixture sets identity and geometry markers, clears them, then requires three identical LCD captures. Verify persistence with the actual normalized Clay Save output. Inspect enlarged nearest-neighbor PNGs for footer/glyph clearance; small previews can hide character strokes.

Run execution/build.py for a clean production bundle, execution/verify.py for artifact identity, MIT notice bytes and fixture exclusion, then check_live.py on each target serially. It checks complete foreground frames and seconds movement, changes emulator battery80→40, restores100, and closes the emulator. This demonstrates native service wiring without claiming physical-watch performance. build_preview.py assembles retained native images.
