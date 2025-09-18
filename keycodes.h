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
