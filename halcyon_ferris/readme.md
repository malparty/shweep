# shweep — Halcyon Ferris rev1

Halcyon Ferris version of [Shweep](../readme.md) (same layout, see the root readme for the keymap).

Hardware: **left = Halcyon rotary encoder module**, **right = Halcyon TFT display module**.
Module support comes from splitkb's code vendored in `users/halcyon_modules`.

## Build & flash

Each half gets its own firmware (same keymap, different module):

```sh
qmk compile -kb splitkb/halcyon/ferris/rev1 -km shweep -e HLC_ENCODER=1     -e TARGET=halcyon_ferris_shweep_left
qmk compile -kb splitkb/halcyon/ferris/rev1 -km shweep -e HLC_TFT_DISPLAY=1 -e TARGET=halcyon_ferris_shweep_right
```

Use `HLC_ENCODER_REV2=1` instead if your encoder module is revision 2.
Flash: double-tap reset, copy the `.uf2` to the `RPI-RP2` drive (left file on the left half, right file on the right half).

## Encoder (left)

| Layer   | Rotate          | Press      |
|---------|-----------------|------------|
| BASE    | Volume −/+      | Mute       |
| MOUSE/2 | Scroll down/up  | Middle click |
| NAV     | Page up/down    | (Mute)     |
| NUM     | Brightness −/+  | Play/Pause |
| RGB     | RGB value −/+   | Next effect |
| others  | as BASE         | as BASE    |

## Display (right)

- Top: layer name, colour coded.
- Middle: **Shweep the sheep** 🐑. It chews on every key press or encoder turn (from both halves),
  chews faster as you type faster, blinks now and then, and falls asleep (zZz) after 30 s idle.
  The pixel art and its colours live in `sheep.h` as editable ASCII; timings are at the top of `display.c`.
- Below the sheep: typing speed (hidden at 0 WPM).
- Bottom: active modifiers (C A G S).

Both halves must be flashed after a change to the split sync settings (activity, WPM) in `config.h`.
