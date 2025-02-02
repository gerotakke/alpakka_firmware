# Changelog

## 0.87.0
- Added new OS mode: HID generic gamepad (aka DirectInput in Windows). See Manual for details.
- Automatic controller restart when changing OS mode.

## 0.86.5
- Show Pico unique hardware id into session log.
- Fixed calibration procedure never completing.
- Added user adjustable presets for touch threshold (5 presets).

_This update requires recalibration._

## 0.86.1
- Added Patreon easter egg.

## 0.86.0
- Created `Console Legacy` profile.
- Any gamepad axis (thumbstick and analog triggers) can be assigned to any button.
- Gyro as a configurable input in the profiles.
    - Gyro to mouse mapping can be customized and inverted.
    - Gyro axis Z is now also available to profiles.
    - Foundations to assign any action to gyro axis (still not functional).
- Gyro modes:
  - `GYRO_MODE_ALWAYS_OFF`
  - `GYRO_MODE_ALWAYS_ON`
  - `GYRO_MODE_TOUCH_OFF`
  - `GYRO_MODE_TOUCH_ON`
- Calibration and non-volatile memory now include gyro axis Z offset.

_This update requires recalibration._

## 0.84.6
- First published version.
