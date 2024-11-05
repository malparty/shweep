// Copyright 2023 Florent Linguenheld (@FLinguenheld)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

enum custom_layers {
    _BASE,
    _BASEWIN,
    _MOUSE,
    _MOUSE2,
    _NAV,
    _NAVWIN,
    _NUM,
    _SYM,
    _SENDSTR,
    _RGB,
    _GAME,
};

enum custom_keys {
    // SEND STRINGS
    SS_SLOW_START = QK_KB_0, // might conflict with MOD TAP!!!
        SS_WQ,
    SS_FAST_START,
        // Matching table `send_string_fast_actions`
        SS_MAILW,
        SS_MAILP,
        SS_LNAME,
        SS_FNAME,
        SS_GITTT,
        SS_GTPF,
        SS_TX,
        SS_NC,
        SS_CK,
        SS_HARO,

        SS_THE,
        SS_AND,

        // NOT in the base table `send_string_fast_actions`
        SS_GITCM,
        SS_GITCMSCI,

        SS_A_GRAVE,
        SS_A_CIRCUMFLEX,
        SS_E_GRAVE,
        SS_E_AIGU,
        SS_E_CIRCUMFLEX,
        SS_I_CIRCUMFLEX,
        SS_O_CIRCUMFLEX,
        SS_U_CIRCUMFLEX,
        SS_U_GRAVE,
        SS_C_CEDI,
    SS_END,
};

enum tap_dance_keys {
    TD_TAB_MOUSE,
    TD_SPC_NAV,
    TD_BSP_NUM,
    TD_ENT_SYM,
    TD_QUOTE_SENDS,

    TD_BROWSER,
    TD_GITFIGMA,
    TD_RECTANGLE,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
    uint16_t held_layer;
} tap_dance_tap_hold_layer_t;
