# Line + APDS9960 five-color V2 — test protocol

This branch is experimental. **Do not merge or run at full speed before validating with the physical Leanbot.**

## Scope
- Line following: 4-bit calibrated `LbIRLine.read()/value()`, no analog. Three steering reductions (50/70/100%), with edge masks 1000 (8) => runLR(0,2000) and 0001 (1) => runLR(2000,0); 40 ms straight hold when mask is zero, then recovery in last known direction.
- Junction detection still relies on `line == 15`; the original junction-counting logic was not changed.
- APDS9960: sequential red/green/blue/off illumination with ambient subtraction, maximum 350 ms sample wait per illumination, classification into Red/Yellow/Green/Blue/Purple/Unknown.
- Original motion/gripper routes preserved. The color-based mission now dispatches Red and Yellow explicitly; unexpected/failed reads are not silently classified as Red.

## Known limitations / blockers
1. `main.cpp` still invokes `di_thang`, `di_lui`, `re_phai`, `re_trai` without definitions in this repository's current source. These may be external Leanbot-Advanced helpers. Compilation will fail unless provided by the build environment. Do not guess their motion units.
2. `followLine()` has an existing `while (LbIRLine.isBlackDetected())` guard. It can exit on `line == 0` before the new line-recovery controller gets to search. The 40 ms hold primarily applies where `followLineMotorOnly()` continues to be called (e.g., junction counting). A future change must separate the desired end-of-segment behavior from lost-line recovery.
3. The 4-bit state-to-turn table preserves the original sketch's steering directions but does not prove actual individual bit ordering. Validate using `LbIRLine.print()` / LEDs on the robot.
4. APDS color channel gains and ratios are provisional. Measure multiple samples for each actual block under the event's lighting and adjust gains/thresholds accordingly. `Unknown` is an intentional safety output.
5. No local hardware compilation/robot test or CI setup was available in this repository.

## Hardware test checklist
1. Perform the existing manual calibration (`LbIRLine.doManualCalibration(TB1A)`).
2. At low speed, move the four line sensors individually across a black strip and note the mask + corresponding turn direction. Do not increase speed if either steering direction is inverted.
3. Test straight lines, gentle curves, sharp corners, junctions, and a short black-line gap; check that junctions are counted once.
4. Use serial debugging to log `ObjRed`, `ObjGreen`, `ObjBlue`, and `Obj_detectColor()` for at least 10 readings per actual Red, Yellow, Green, Blue, Purple block and for no block. Record both successes and Unknowns.
5. Compile the full project with any required Leanbot-Advanced motion-helper definitions, then run the complete mission at conservative speed.

## Tunables
- Line: `LINE_TURN_SMALL`, `LINE_TURN_MEDIUM`, `LINE_TURN_LARGE`, `LINE_LOSS_HOLD_MS`
- Color: `COLOR_GAIN_R/G/B`, `COLOR_MIN_SIGNAL`, `COLOR_TIMEOUT_MS` and ratios inside `Obj_detectColor()`

## Experimental fixed A/B/D mission (added 2026-11-03)
- `main.cpp` now contains `abdExecuteMission()` and a replacement `loop()` with fixed colors A2 Yellow, B4 Green, D3 Blue and reserve S1 Green / S3 Blue.
- **Revised START: center of CTRL cell, robot facing SOUTHWEST.** `abdExitCtrl()` moves approximately 287 mm (half the nominal cell diagonal) to the grid vertex `(2,3)`; this is an **unverified geometric hypothesis**, NOT the physical starting setup confirmed by the organizers. Validate position, heading, clearance and access to line before moving. The earlier assumption at `(0,1)` is obsolete.
- Nominal 4-foot board = 1219.2 mm side; conceptual grid pitch = 406.4 mm. Encoder and junction thresholds, reserve access points, approach distances, and 45-degree motions are estimates that require physical calibration.
- Route implemented: B4 push -> D3 push -> S3 Blue -> D3 place -> S1 Green -> B4 place -> A2 Yellow -> CTRL place.
- Digital line-junction navigation checks three consecutive full-black masks and has distance/clock safety limits; it does NOT replace a measured map.
- Sensor-color classifier is retained, but this mission intentionally uses known colors without calling APDS during execution.
- The other robot's path is not modeled. Do not run concurrently before collision-clearance testing.
- Replacing the original hardcoded `loop()` removed references to undefined `di_thang`, `di_lui`, `re_phai`, and `re_trai` helpers. Hardware compile is still unverified.

## Simplified direct-loop mission (2026-11-03)
- The previous `abd*` coordinate/heading navigation model was **removed**. `loop()` now directly sequences existing Leanbot APIs: `LbMotion.runLRrpm`, `LbMotion.waitDistanceMm`, `LbMotion.waitRotationDeg`, `followLineUntilNthJunction`, and gripper helpers.
- Start: CTRL, facing southwest. First command drives straight 287 mm (estimated) before a 45-degree right turn to the bottom line.
- Task sequence is B Green push, D Blue push, S3 Blue pickup/delivery, S1 Green pickup/delivery, A Yellow pickup/CTRL delivery.
- **No successful compilation or physical navigation has been verified**. Junction counts, turning geometry, reserve approach, and hard-coded distances are PROVISIONAL; original `followLineUntilNthJunction` may block indefinitely when a junction is missed. Test one leg at a time with clear space and low motor speed; do not run the second robot simultaneously yet.
