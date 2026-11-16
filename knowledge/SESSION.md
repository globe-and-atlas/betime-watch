# BeTime 0.4.1 store release · 2026-11-10

**Goal:** Publish original Time Round support and its native gallery still.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Published and publicly verified.

The public download matches the independently verified six-target build at SHA-256 994c53c8e1203a825b5920339287372ec411f6eff64113304554673ea4310d27. The Time Round gallery displays the native 180×180 Weather, Steps, and London capture with an exact pixel match. The original Time 2 and Round 2 GIFs remain first and match their prior bytes. The public compatibility copy now includes Time Round. Proof is in release/0.4.1/public-proof.json.

S.D.G.: Brings BeTime to original Time Round owners with a native preview before install.
Feynman gap: Live Weather and Steps on physical Time Round hardware remain unverified.

# BeTime original Time Round preview · 2026-11-10

**Goal:** Add the original Time Round and verify the three-tile layout at native size.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Local 0.4.1 ready for review. Public version is 0.4.0.

The 180×180 Chalk layout raises the two outer tiles to follow the curve. Weather 64°F, Steps 8426, and London 3:42 PM fit in the native capture at media/native-time-round.png. A stress fixture also fits Auckland 12:42 PM, six-digit steps, and a signed three-digit temperature. The fixture proves minute ticks with Seconds absent. The circle proof uses exact integer arithmetic to avoid the legacy Chalk fixture fault.

The clean local bundle is dist/BeTime-0.4.1.pbw, SHA-256 994c53c8e1203a825b5920339287372ec411f6eff64113304554673ea4310d27. Its six targets include the existing five with the original UUID and message keys.

S.D.G.: Brings the configurable BeTime desktop to original Time Round owners.
Feynman gap: The readings come from native emulator fixtures. Live weather and Pebble Health on a physical Time Round remain unverified.

# BeTime 0.4.0 store release · 2026-11-10

**Goal:** Ship the approved compact build with native gallery screenshots.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Published and publicly verified.

The public PBW matches the independently verified five-target build at SHA-256 ac58080d3faaba70e45416f00d84517ed855eb37409721cc1e084f9c48a2993a. The Pebble 2, 2 Duo, and Time/Time Steel galleries each show their own native Weather, Steps, and London still. The public stills match the native captures pixel for pixel. Time 2 and Round 2 retain the same galleries with both original animations first. Store copy names all supported models and explains the monochrome palette. Proof is in release/0.4.0/public-proof.json.

S.D.G.: Brings BeTime's three configurable desktop tiles to the smaller rectangular Pebbles.
Feynman gap: Emulator renders use sample values. Live health and paired-phone weather on compact hardware remain unverified.

# BeTime compact preview · 2026-11-10

**Goal:** Preview BeTime on Pebble 2, Pebble 2 Duo, and Pebble Time/Time Steel.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Local 0.4.0 ready for visual review. Public version is 0.3.1.

The 144×168 layout retains three tiles. Compact labels use regular Gothic 14 so Weather fits. Diorite and Flint use a fixed monochrome palette. Basalt keeps the desktop palettes and custom colors. The preview sheet is media/native-compact.png. Native captures and minute-tick evidence are in .tmp/compact-native. All three compact platforms passed the Weather, Steps, and London case. Supplemental fixtures exercised a six-digit step count, a signed three-digit temperature, 24-hour time, and a two-digit PM world time. Native Time 2 and Round 2 comparison frames match the 0.3.1 frames pixel for pixel.

The clean independently reviewed bundle is dist/BeTime-0.4.0.pbw, SHA-256 ac58080d3faaba70e45416f00d84517ed855eb37409721cc1e084f9c48a2993a. It targets Emery, Gabbro, Diorite, Flint, and Basalt with the existing UUID and message keys.

S.D.G.: Brings BeTime's configurable desktop to owners of the smaller rectangular Pebbles.
Feynman gap: The captures use emulator fixtures. Live health and paired-phone weather on the compact watches remain unverified.

# BeTime 0.3.1 store release · 2026-11-10

**Goal:** Publish the approved World Clock globe for Time 2 and Round 2.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Published and publicly verified on both models.

BeTime 0.3.1 ships the simpler Europe and Africa globe with the thinner rim and ocean highlight. The public PBW matches the independently reviewed production build at SHA-256 86350f66c5e6a380122f05cf4fbe39994b22bbe1adcd14a564773b209e3b4f32. The Weather, Steps, and London still in each gallery matches the approved native pixels. Both original animations remain first. Evidence is in release/0.3.1/public-proof.json.

S.D.G.: Gives BeTime wearers a clearer World Clock icon and a preview of it before installing.
Feynman gap: Both emulators and the public assets passed verification. This release had no physical-watch check.

# BeTime 0.3.0 store release · 2026-11-10

**Goal:** Publish BeTime 0.3.0 for Time 2 and Round 2 with the revised World Clock globe.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Published and publicly verified on both supported models.

The clean bundle is dist/BeTime-0.3.0.pbw, SHA-256 a46344214923c9bd0bb1044451ee8deac1d55c6a92ec32a65d2618ce26ebeb4a. Independent review verified the production binaries and minute-only update mode on both emulators. Both public galleries retain the original animation first. The fifth still on each model now shows Weather, Steps, and London. Both public covers match the original bytes. Both public stills match the native capture pixels. Proof is in release/0.3.0/public-proof.json.

S.D.G.: Gives BeTime wearers three configurable desktop tiles and shows the revised World Clock globe before install.
Feynman gap: Physical paired-phone permissions and background delivery remain unverified.

# Desktop data tiles — 2026-11-10

**Goal:** Add Weather, Phone, and World Clock to the three independent BeTime slots.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Local 0.3.0 native preview ready. The public listing serves 0.1.0.

Weather shows current conditions and a temperature from Open-Meteo. The phone rounds the coordinates to two decimal places. The watch retains readings for up to 90 minutes. Phone reflects the Pebble app connection. World Clock offers UTC and ten named cities. The watch applies the phone's cached offset transition at the correct minute. The watch sends refresh requests on its existing minute callback when a connected tile needs data.

Host tests, the live provider parser check, and independent review passed. Native Time 2 and Round 2 matrices verified all three tiles in each position, minute/second selection, link states, stale weather, an offset transition, UTC, and cache persistence. The final local production bundle is dist/BeTime-0.3.0.pbw, SHA-256 c1acaac6413f9ee37c4141aa0290e42adbc195fcaba4b650bdec8e645359f8dd. Preview: media/native-desktop-data.png. Evidence: .tmp/desktop-data/verification.json and .tmp/production-verification.json.

S.D.G.: Makes the desktop slots useful for local weather, phone status, and time elsewhere.
Feynman gap: The watch behavior and phone-side processing passed separate checks. Physical paired-phone permissions and background delivery remain open.

# Desktop tile options — 2026-11-10

**Goal:** Let users customize the bottom row and choose a lower update rate by replacing Seconds.
**Agent:** OpenAI Codex
**Handoff-from:** OpenAI Codex
**Handoff-type:** continuation
**Status:** Local 0.2.0 build ready for review. Store release is 0.1.0.

Each slot accepts Battery, Date, Seconds, Steps, or Empty. Older settings retain Power, Today, and Clock. Steps reads daily Pebble Health data only when selected. A missing count displays --. The face subscribes to minute ticks when Seconds is absent.

Both native emulators verified the original visible pixels, Steps in each position, mixed Seconds and Steps, empty slots, missing counts, persisted choices, and invalid value rejection. The test-only marker confirmed the selected timer mask. Host checks, actual Clay Save/Cancel/reload, and clean PBW verification passed. See media/native-tile-options.png and .tmp/tiles/verification.json. Local PBW is dist/BeTime-0.2.0.pbw with SHA-256 51006462b5cb989228251ada36a7020ffb22d68a5f1c2dd538cd3cffefe0ad12.

S.D.G.: Lets BeTime wearers keep the desktop look and choose readouts that suit their day.
Feynman gap: Native emulators confirm tick selection and fixture display. Real step data and battery endurance remain physical-watch checks.

---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-11-08T17:24:01-06:00"
---
# BeTime handoff
Goal: build approved BeTime for Time 2 and Round 2 with actual Haiku icons, color options, and 12/24-hour time.
Agent: OpenAI Codex
Handoff-from: none
Handoff-type: new-project
Status: published and publicly verified. Independent release review passed.

Production: dist/BeTime-0.1.0.pbw. UUID f4d2048a-62f7-4642-9997-5a6967da1ff8. SHA256 a78624572c2d8b0106268c5c403920588087f8da05690623b0ad7aad9162302c. Targets Emery200x228 and Gabbro260x260 only. Source retained on local codex/betime branch, uncommitted.

Native layout: short gold Clock tab, gray beveled window, blue desktop, real PowerStatus/Calendar/Clock assets. Power shows battery, Today shows date, Clock shows seconds. Six palettes and custom desktop/window/text/tab colors. Watch default, 12-hour, 24-hour. Date short default, Off/ISO/US/EU options. Both MIT notices verified in both packaged resource packs.

Verification: pytest2 passed; full-day host formatting and geometry pass; real Clay Save/Cancel/reload pass. Native21 cases per target, including palette/custom, midnight/noon/23:59, dates, battery0/100, normalized Save→reinstall→invalid setting rejection. Time2 footer moved up7px and final suite rerun. Production seconds and battery80→40 observed on both emulators. Production exact target binaries, resource licenses, and fixture-symbol exclusion independently reviewed. Native renders: media/native-comparison.png and media/native-palettes.png. Logs/proof: .tmp/native/ and .tmp/production-verification.json. Emulator battery restored100%; emulator stopped.

Store release: https://apps.repebble.com/f77f655a2c9f4b72a2f39e58. The public PBW matches the reviewed SHA. The Time 2 and Round 2 galleries each begin with an exact seven-frame GIF covering six presets plus custom, followed by four lossless single-frame GIF stills. The lossless media correction bypasses store PNG quantization. All eight public stills match native capture pixels exactly, both original cover URLs and bytes match, and both public gallery orders were verified after the listing cache refreshed. Evidence is in release/lossless-proof.json.

Next: physical-watch review when hardware is available.

S.D.G.: Pebble owners get a readable clock with the familiar character of a BeOS desktop.
Feynman gap: native emulator behavior is verified; physical-watch legibility and battery endurance remain unmeasured.
