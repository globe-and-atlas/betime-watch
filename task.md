---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-08T17:11:48-06:00"
---
# Build BeTime

## Contract
- Directive: directives/build_watchface.md.
- Scripts: execution/build.py, build_fixture.py, capture_native.py, verify.py.
- Output: dist/BeTime-0.1.0.pbw; media/native-comparison.png.
- Targets: Emery 200x228; Gabbro 260x260.
- Initial build scope preceded publication authorization. Never read secrets or SDK lock files.

## Validation
- [x] Emery production binary builds.
- [x] Gabbro production binary builds.
- [x] Power uses the sourced PowerStatus image.
- [x] Today uses the sourced Calendar image.
- [x] Clock uses the sourced Clock image.
- [x] MIT notices are included in the bundle.
- [x] Default layout preserves the approved desktop design.
- [x] Six palettes are selectable.
- [x] Custom desktop color is selectable.
- [x] Custom window color is selectable.
- [x] Custom tab color is selectable.
- [x] Custom text color is selectable.
- [x] 12-hour time shows midnight as 12 AM.
- [x] 12-hour time shows noon as 12 PM.
- [x] 24-hour time shows midnight as 00.
- [x] Watch-default mode follows the system preference.
- [x] Power shows battery percentage.
- [x] Today shows the date.
- [x] Date can be hidden.
- [x] ISO date is selectable.
- [x] US date is selectable.
- [x] European date is selectable.
- [x] Clock shows current seconds.
- [x] Real Clay Save normalizes values.
- [x] Real Clay Cancel preserves values.
- [x] Native saved settings survive reinstall.
- [x] Invalid native settings are rejected.
- [x] Both native layouts pass bounds checks.
- [x] Native palette screenshots are inspected.
- [x] Production excludes fixture controls.
- [x] Independent review passes.

Final native matrices: 21 cases per model. Production live seconds and battery updates verified on both emulators. Independent review passed.

## Store release
- [x] Public PBW matches the reviewed BeTime 0.1.0 build.
- [x] Time 2 gallery starts with the palette GIF.
- [x] Round 2 gallery starts with the palette GIF.
- [x] Time 2 gallery contains four native stills.
- [x] Round 2 gallery contains four native stills.
- [x] Each GIF shows six presets plus one custom example.
- [x] Independent release review passes.

## Lossless store stills
- [x] Time 2 replacement stills match native RGB pixels.
- [x] Round 2 replacement stills match native RGB pixels.
- [x] Round 2 public gallery retains its animated cover in slot one.
- [x] Time 2 public gallery serves the saved lossless stills.

## Configurable desktop tiles — 2026-11-10

- [x] Each of the left, middle, and right slots can select Battery, Date, Seconds, Steps, or Empty.
- [x] Existing settings retain Battery, Date, Seconds by default.
- [x] A configuration with Seconds requests second ticks.
- [x] A configuration without Seconds requests minute ticks.
- [x] Steps displays a placeholder when Pebble Health data is unavailable.
- [x] Native Time 2 tile captures pass review.
- [x] Native Round 2 tile captures pass review.
- [x] Independent review passes.

## Weather, Phone, and World Clock

- [x] Each new tile can be selected in each desktop slot.
- [x] An arrangement without Seconds requests minute ticks.
- [x] Weather has a stale-data fallback.
- [x] World Clock follows the selected city's offset transition.
- [x] Time 2 native captures pass review.
- [x] Round 2 native captures pass review.
- [x] Independent review passes.

## Desktop data store release 0.3.0

- [x] The revised globe is visually legible on Time 2.
- [x] The revised globe is visually legible on Round 2.
- [x] The public PBW matches the reviewed production bundle.
- [x] The original Time 2 animation is first.
- [x] The original Round 2 animation is first.
- [x] The Time 2 gallery contains the new native still.
- [x] The Round 2 gallery contains the new native still.
- [x] Independent verification passes.

## BeTime 0.3.1 store release

- [x] The public 0.3.1 PBW matches the clean production build.
- [x] The Time 2 gallery shows the approved globe in the Weather, Steps, and London still.
- [x] The Round 2 gallery shows the approved globe in the Weather, Steps, and London still.
- [x] The original Time 2 GIF remains first.
- [x] The original Round 2 GIF remains first.
- [x] Independent release verification passes.

## Compact rectangular BeTime preview

- [x] Diorite has a 144×168 native capture with Weather, Steps, London.
- [x] Flint has a 144×168 native capture with Weather, Steps, London.
- [x] Basalt has a 144×168 native capture with Weather, Steps, London.
- [x] Each compact capture shows the three values without truncation.
- [x] The compact fixture confirms minute ticks with Seconds absent.
- [x] Time 2 and Round 2 retain their native layout.
- [x] The clean preview PBW contains the five intended targets.
- [x] Independent review passes.

## BeTime 0.4.0 store release

- [x] The public PBW matches the approved 0.4.0 bundle.
- [x] The Pebble 2 gallery includes its native Weather, Steps, and London still.
- [x] The Pebble 2 Duo gallery includes its native Weather, Steps, and London still.
- [x] The Time and Time Steel gallery includes its native Weather, Steps, and London still.
- [x] The original Time 2 animation remains first.
- [x] The original Round 2 animation remains first.
- [x] Independent release verification passes.

## Original Time Round preview

- [x] Chalk produces a 180×180 native capture of Weather, Steps, London.
- [x] The full Weather label is visible.
- [x] The full Steps label is visible.
- [x] The full London label is visible.
- [x] The three sample readings clear the curved screen edge.
- [x] Chalk updates on minute ticks with Seconds absent.
- [x] The clean preview PBW retains the five 0.4.0 targets.
- [x] The clean preview PBW contains Chalk.
- [x] Independent preview review passes.

## BeTime 0.4.1 store release

- [x] The public PBW matches the approved six-target bundle.
- [x] The Time Round gallery shows the native Weather, Steps, and London capture.
- [x] The original Time 2 GIF is first in its gallery.
- [x] The original Round 2 GIF is first in its gallery.
- [x] Independent release-input verification passes.
