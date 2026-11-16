# Errors

Record deterministic errors, root causes, and fixes here.

## 2026-11-10 · Chalk fixture faults in the circle proof

- **Symptom:** The original Time Round fixture showed stale tile values or “BeTime is not responding” after proof was enabled.
- **Cause:** The instrumented fixture's fault PC resolved to `__ieee754_sqrt`, called by `hypot` in the circle bounds proof.
- **Fix:** Compare squared distances in integer half-pixel units. Capture Weather and World Clock as separate complete AppMessage packets, matching the phone companion.
- **Graduated to:** `bt_bounds` and the host geometry test.
