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
  A_CIRCUMFLEX,
  E_GRAVE,
  E_AIGU,
  E_CIRCUMFLEX,
  I_CIRCUMFLEX,
  U_CIRCUMFLEX,
  U_GRAVE,
  O_CIRCUMFLEX,
  C_CEDI,

  LBRC_SFT,
  RBRC_SFT,
  LBRC,
  RBRC,

  COMBO_RCHE,
  COMBO_LCHE,
  COMBO_RPAR,
  COMBO_LPAR,

  COMBO_CAPS,
  COMBO_QUOTE,
  COMBO_DQUOTE,
  COMBO_BQUOT,

  COMBO_MINUS,
  COMBO_EQ,
  COMBO_PLUS,
  COMBO_UNDSCORE,

  COMBO_PIPE,
  COMBO_COL,
  COMBO_SEMICOL,
  COMBO_TILDE,
  COMBO_BCKSLASH,

  COMBO_QUIT,
  COMBO_THE,
  COMBO_AND,

  // Windows Layout
  COMBO_A_GRAVE_WIN,
  COMBO_A_CIRCUMFLEX_WIN,
  COMBO_E_GRAVE_WIN,
  COMBO_E_AIGU_WIN,
  COMBO_E_CIRCUMFLEX_WIN,
  COMBO_U_CIRCUMFLEX_WIN,
  COMBO_O_CIRCUMFLEX_WIN,
  COMBO_QUOTE_WIN,
  COMBO_EQ_WIN,
  COMBO_COL_WIN,

  /* Just to replace the define in config.h */
  COMBO_LENGTH,
};
uint16_t COMBO_LEN = COMBO_LENGTH;

/* Sequences fo keys */
/* Diacritics */
const uint16_t PROGMEM combo_a_grave[] = {MT(MOD_LGUI, KC_A), KC_X, COMBO_END};
const uint16_t PROGMEM combo_a_circumflex[] = {MT(MOD_LGUI, KC_A), KC_ESC, COMBO_END};
const uint16_t PROGMEM combo_e_grave[] = {MT(MOD_RCTL, KC_E), KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_e_aigu[] = {MT(MOD_RCTL, KC_E), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_e_circumflex[] = {MT(MOD_RCTL, KC_E), KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_i_circumflex[] = {MT(MOD_RALT, KC_I), KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_u_circumflex[] = {MT(MOD_RCTL, KC_E), KC_U, COMBO_END};
const uint16_t PROGMEM combo_u_grave[] = {KC_U, MT(MOD_RALT, KC_I), COMBO_END};
const uint16_t PROGMEM combo_o_circumflex[] = {MT(MOD_RGUI, KC_O), LT(_RGB, KC_SLSH), COMBO_END};
const uint16_t PROGMEM combo_c_cedi[] = {KC_C, MT(MOD_LSFT, KC_T), COMBO_END};

const uint16_t PROGMEM combo_caps[] = {KC_W, KC_F, KC_P, COMBO_END};

/* ' " ` */
const uint16_t PROGMEM combo_quote[] = {MT(MOD_LALT, KC_R),MT(MOD_LCTL, KC_S),MT(MOD_LSFT, KC_T), COMBO_END};
const uint16_t PROGMEM combo_dquote[] = {KC_X, KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_bquot[] = {KC_L, KC_U, KC_Y, COMBO_END};

/* [] () <> {} */
const uint16_t PROGMEM combo_rbrc_sft[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_lbrc_sft[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_rbrc[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_lbrc[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM combo_lpar[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM combo_rpar[] = {KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_lche[] = {KC_H, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_rche[] = {KC_COMM, KC_DOT, COMBO_END};

/* - + = _ */
const uint16_t PROGMEM combo_minus[] = {MT(MOD_LALT, KC_R), KC_W, COMBO_END};
const uint16_t PROGMEM combo_eq[] = {MT(MOD_LCTL, KC_S), KC_F, COMBO_END};
const uint16_t PROGMEM combo_plus[] = {MT(MOD_LSFT, KC_T), KC_P, COMBO_END};
const uint16_t PROGMEM combo_undscore[] = {MT(MOD_LALT, KC_R), KC_X, COMBO_END};

/* ; : ~ | \ */
const uint16_t PROGMEM combo_semicol[] = {KC_H, KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_col[] = {MT(MOD_RSFT, KC_N), MT(MOD_RCTL, KC_E), MT(MOD_RALT, KC_I), COMBO_END};
const uint16_t PROGMEM combo_tilde[] = {MT(MOD_RALT, KC_I), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_pipe[] = {MT(MOD_RSFT, KC_N), KC_L, COMBO_END};
const uint16_t PROGMEM combo_bckslash[] = {MT(MOD_RSFT, KC_N), KC_H, COMBO_END};

const uint16_t PROGMEM combo_quit[] = {KC_ESC, KC_B, COMBO_END};
const uint16_t PROGMEM combo_the[] = {KC_X, KC_D, COMBO_END};
const uint16_t PROGMEM combo_and[] = {KC_H, KC_DOT, COMBO_END};

/* WINDOWS Layer */
/* Diacritics */
const uint16_t PROGMEM combo_a_grave_win[] = {MT(MOD_LCTL, KC_A), KC_X, COMBO_END};
const uint16_t PROGMEM combo_a_circumflex_win[] = {MT(MOD_LCTL, KC_A), KC_ESC, COMBO_END};
const uint16_t PROGMEM combo_e_grave_win[] = {MT(MOD_RGUI, KC_E), KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_e_aigu_win[] = {MT(MOD_RGUI, KC_E), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_e_circumflex_win[] = {MT(MOD_RGUI, KC_E), KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_u_circumflex_win[] = {MT(MOD_RGUI, KC_E), KC_U, COMBO_END};
const uint16_t PROGMEM combo_o_circumflex_win[] = {MT(MOD_RCTL, KC_O), LT(_RGB, KC_SLSH), COMBO_END};
const uint16_t PROGMEM combo_quote_win[] = {MT(MOD_LALT, KC_R),MT(MOD_LGUI, KC_S),MT(MOD_LSFT, KC_T), COMBO_END};
const uint16_t PROGMEM combo_eq_win[] = {MT(MOD_LGUI, KC_S), KC_F, COMBO_END};
const uint16_t PROGMEM combo_col_win[] = {MT(MOD_RSFT, KC_N), MT(MOD_RGUI, KC_E), MT(MOD_RALT, KC_I), COMBO_END};

/* Sequences fo keys */
combo_t key_combos[] = {
    [A_GRAVE] = COMBO(combo_a_grave, SS_A_GRAVE),
    [A_CIRCUMFLEX] = COMBO(combo_a_circumflex, SS_A_CIRCUMFLEX),
    [E_GRAVE] = COMBO(combo_e_grave, SS_E_GRAVE),
    [E_AIGU] = COMBO(combo_e_aigu, SS_E_AIGU),
    [E_CIRCUMFLEX] = COMBO(combo_e_circumflex, SS_E_CIRCUMFLEX),
    [I_CIRCUMFLEX] = COMBO(combo_i_circumflex, SS_I_CIRCUMFLEX),
    [U_CIRCUMFLEX] = COMBO(combo_u_circumflex, SS_U_CIRCUMFLEX),
    [U_GRAVE] = COMBO(combo_u_grave, SS_U_GRAVE),
    [O_CIRCUMFLEX] = COMBO(combo_o_circumflex, SS_O_CIRCUMFLEX),
    [C_CEDI] = COMBO(combo_c_cedi, LOPT(KC_C)),

    [COMBO_CAPS] = COMBO(combo_caps, KC_CAPS),
    [COMBO_QUOTE] = COMBO(combo_quote, KC_QUOTE),
    [COMBO_DQUOTE] = COMBO(combo_dquote, LSFT(KC_QUOTE)),

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

    [COMBO_BQUOT] = COMBO(combo_bquot, KC_GRV),
    [COMBO_PIPE] = COMBO(combo_pipe, KC_PIPE),
    [COMBO_COL] = COMBO(combo_col, KC_COLN),
    [COMBO_SEMICOL] = COMBO(combo_semicol, KC_SCLN),
    [COMBO_TILDE] = COMBO(combo_tilde, KC_TILD),
    [COMBO_BCKSLASH] = COMBO(combo_bckslash, KC_BSLS),

    [COMBO_QUIT] = COMBO(combo_quit, LGUI(KC_Q)),
    [COMBO_THE] = COMBO(combo_the, SS_THE),
    [COMBO_AND] = COMBO(combo_and, SS_AND),

    /* WINDOW layout */
    [COMBO_A_GRAVE_WIN] = COMBO(combo_a_grave_win, SS_A_GRAVE),
    [COMBO_A_CIRCUMFLEX_WIN] = COMBO(combo_a_circumflex_win, SS_A_CIRCUMFLEX),
    [COMBO_E_GRAVE_WIN] = COMBO(combo_e_grave_win, SS_E_GRAVE),
    [COMBO_E_AIGU_WIN] = COMBO(combo_e_aigu_win, SS_E_AIGU),
    [COMBO_E_CIRCUMFLEX_WIN] = COMBO(combo_e_circumflex_win, SS_E_CIRCUMFLEX),
    [COMBO_U_CIRCUMFLEX_WIN] = COMBO(combo_u_circumflex_win, SS_U_CIRCUMFLEX),
    [COMBO_O_CIRCUMFLEX_WIN] = COMBO(combo_o_circumflex_win, SS_O_CIRCUMFLEX),
    [COMBO_QUOTE_WIN] = COMBO(combo_quote_win, KC_QUOTE),
    [COMBO_EQ_WIN] = COMBO(combo_eq_win, KC_EQL),
    [COMBO_COL_WIN] = COMBO(combo_col_win, KC_COLN),
};
