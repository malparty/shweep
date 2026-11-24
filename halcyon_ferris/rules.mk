# Halcyon modules (encoder on the left, TFT display on the right).
# Code vendored in users/halcyon_modules. Each half needs its own build:
#   left : qmk compile -kb splitkb/halcyon/ferris/rev1 -km shweep -e HLC_ENCODER=1
#   right: qmk compile -kb splitkb/halcyon/ferris/rev1 -km shweep -e HLC_TFT_DISPLAY=1
USER_NAME := halcyon_modules
ENCODER_ENABLE = yes
ENCODER_MAP_ENABLE = yes

INTROSPECTION_KEYMAP_C += features/combo.c # Replace SRC, see issue #21137
SRC += display.c
WPM_ENABLE = yes # typing speed for the display sheep

REPEAT_KEY_ENABLE = yes

MOUSEKEY_ENABLE = yes
AUTO_SHIFT_ENABLE = no
AUTO_SHIFT_MODIFIERS = no
COMBO_ENABLE = yes
LEADER_ENABLE = no
TAP_DANCE_ENABLE = yes

# Require hex input which prevents OPT left/right shortcuts.
UNICODE_ENABLE = no

SEND_STRING_ENABLE = yes

RGB_MATRIX_CUSTOM_USER = yes
