// Copyright 2023 Florent Linguenheld (@FLinguenheld)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keycodes.h"

/* How to :
        - Add an entry in the enum
        - Create a new sequence of keys
        - Link the enum to this sequence in the key_combos table

   For macros :
        - Same steps
        - And add the macro in the process_combo_event() function
*/

enum combos {
  A_GRAVE,
  E_GRAVE,
  E_AIGU,
  E_CIRCUMFLEX,
  U_GRAVE,
  C_CEDI,

  // Horizontals
  LBRC_SFT,
  RBRC_SFT,
  LBRC,
  RBRC,

  COMBO_RCHE,
  COMBO_LCHE,
  COMBO_RPAR,
  COMBO_LPAR,

  COMBO_ESC,
  COMBO_ENTER,
  COMBO_BACKSPACE,
  COMBO_CAPS,

  // Verticals
  COMBO_MINUS,
  COMBO_EQ,
  COMBO_PLUS,
  COMBO_UNDSCORE,
  COMBO_SQUOT,
  COMBO_DQUOT,

  COMBO_BQUOT,
  COMBO_PIPE,
  COMBO_COL,
  COMBO_SEMICOL,
  COMBO_TILDE,
  COMBO_BCKSLASH,

  /* Just to replace the define in config.h */
  COMBO_LENGTH,
};
uint16_t COMBO_LEN = COMBO_LENGTH;

/* Sequences fo keys */
/* RIGHT HAND */
const uint16_t PROGMEM combo_a_grave[] = {MT(MOD_LGUI, KC_A), KC_X, COMBO_END};
const uint16_t PROGMEM combo_e_grave[] = {MT(MOD_RCTL, KC_E), KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_e_aigu[] = {MT(MOD_RCTL, KC_E), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_e_circumflex[] = {KC_H, MT(MOD_RCTL, KC_E), KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_u_grave[] = {KC_U, MT(MOD_RALT, KC_I), COMBO_END};
const uint16_t PROGMEM combo_c_cedi[] = {KC_C, MT(MOD_LSFT, KC_T), COMBO_END};

// Horizontals
const uint16_t PROGMEM combo_esc[] = {KC_W, KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_enter[] = {MT(MOD_LALT, KC_R),MT(MOD_LCTL, KC_S),MT(MOD_LSFT, KC_T), COMBO_END};
const uint16_t PROGMEM combo_backspace[] = {KC_X, KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_caps[] = {KC_Z, MT(MOD_LGUI, KC_A), COMBO_END};

const uint16_t PROGMEM combo_rbrc_sft[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_lbrc_sft[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_rbrc[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_lbrc[] = {KC_W, KC_F, COMBO_END};

// Verticals
const uint16_t PROGMEM combo_minus[] = {MT(MOD_LALT, KC_R), KC_W, COMBO_END};
const uint16_t PROGMEM combo_eq[] = {MT(MOD_LCTL, KC_S), KC_F, COMBO_END};
const uint16_t PROGMEM combo_plus[] = {MT(MOD_LSFT, KC_T), KC_P, COMBO_END};
const uint16_t PROGMEM combo_undscore[] = {MT(MOD_LALT, KC_R), KC_X, COMBO_END};
const uint16_t PROGMEM combo_squot[] = {MT(MOD_LCTL, KC_S), KC_C, COMBO_END};
const uint16_t PROGMEM combo_dquot[] = {MT(MOD_LSFT, KC_T), KC_D, COMBO_END};

/* LEFT HAND */
// Horizontals
const uint16_t PROGMEM combo_lpar[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM combo_rpar[] = {KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_lche[] = {KC_H, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_rche[] = {KC_COMM, KC_DOT, COMBO_END};

// Verticals
const uint16_t PROGMEM combo_semicol[] = {MT(MOD_RSFT, KC_N), KC_L, COMBO_END};
const uint16_t PROGMEM combo_col[] = {MT(MOD_RCTL, KC_E), KC_U, COMBO_END};
const uint16_t PROGMEM combo_tilde[] = {MT(MOD_RALT, KC_I), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_bquot[] = {MT(MOD_RSFT, KC_N), KC_H, COMBO_END};
const uint16_t PROGMEM combo_pipe[] = {MT(MOD_RCTL, KC_E), KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_bckslash[] = {MT(MOD_RALT, KC_I), KC_DOT, COMBO_END};

/* Sequences fo keys */
combo_t key_combos[] = {
    [A_GRAVE] = COMBO(combo_a_grave, SS_A_GRAVE),
    [E_GRAVE] = COMBO(combo_e_grave, SS_E_GRAVE),
    [E_AIGU] = COMBO(combo_e_aigu, SS_E_AIGU),
    [E_CIRCUMFLEX] = COMBO(combo_e_circumflex, SS_E_CIRCUMFLEX),
    [U_GRAVE] = COMBO(combo_u_grave, SS_U_GRAVE),
    [C_CEDI] = COMBO(combo_c_cedi, LOPT(KC_C)),

    [COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_ENTER] = COMBO(combo_enter, KC_ENT),
    [COMBO_BACKSPACE] = COMBO(combo_backspace, KC_BSPC),
    [COMBO_CAPS] = COMBO(combo_caps, KC_CAPS),

    [LBRC_SFT] = COMBO(combo_lbrc_sft, LSFT(KC_LBRC)),
    [RBRC_SFT] = COMBO(combo_rbrc_sft, LSFT(KC_RBRC)),
    [LBRC] = COMBO(combo_lbrc, KC_LBRC),
    [RBRC] = COMBO(combo_rbrc, KC_RBRC),

    [COMBO_LCHE] = COMBO(combo_lche, LSFT(KC_COMM)),
    [COMBO_RCHE] = COMBO(combo_rche, LSFT(KC_DOT)),
    [COMBO_LPAR] = COMBO(combo_lpar, LSFT(KC_9)),
    [COMBO_RPAR] = COMBO(combo_rpar, LSFT(KC_0)),

    [COMBO_MINUS] = COMBO(combo_minus, KC_MINS),
    [COMBO_EQ] = COMBO(combo_eq, KC_EQL),
    [COMBO_PLUS] = COMBO(combo_plus, LSFT(KC_EQL)),
    [COMBO_UNDSCORE] = COMBO(combo_undscore, LSFT(KC_MINS)),
    [COMBO_SQUOT] = COMBO(combo_squot, KC_QUOT),
    [COMBO_DQUOT] = COMBO(combo_dquot, LSFT(KC_QUOT)),

    [COMBO_BQUOT] = COMBO(combo_bquot, KC_GRV),
    [COMBO_PIPE] = COMBO(combo_pipe, KC_PIPE),
    [COMBO_COL] = COMBO(combo_col, KC_COLN),
    [COMBO_SEMICOL] = COMBO(combo_semicol, KC_SCLN),
    [COMBO_TILDE] = COMBO(combo_tilde, KC_TILD),
    [COMBO_BCKSLASH] = COMBO(combo_bckslash, KC_BSLS),
};
