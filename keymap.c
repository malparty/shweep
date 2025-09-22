#include QMK_KEYBOARD_H
#include "keycodes.h"


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

// Tap Dance
void tap_dance_tap_hold_layer_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_layer_t *tap_hold = (tap_dance_tap_hold_layer_t *)user_data;

    if (state->pressed && !layer_state_is(tap_hold->hold)) {
        layer_on(tap_hold->hold);
        tap_hold->held_layer = tap_hold->hold;
    }
}

void tap_dance_tap_hold_layer_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_layer_t *tap_hold = (tap_dance_tap_hold_layer_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    } else if (tap_hold->held_layer) {
        layer_clear();
        tap_hold->held_layer = 0;
    }
}

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_layer_t *tap_hold = (tap_dance_tap_hold_layer_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_layer_t *tap_hold = (tap_dance_tap_hold_layer_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_layer_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    [TD_BROWSER] = ACTION_TAP_DANCE_TAP_HOLD(LGUI(KC_3), LGUI(KC_6)),
    [TD_GITFIGMA] = ACTION_TAP_DANCE_TAP_HOLD(LGUI(KC_2), LGUI(KC_5)),
    [TD_RECTANGLE] = ACTION_TAP_DANCE_TAP_HOLD(LGUI(LOPT(LCTL(KC_LEFT))), LOPT(LCTL(KC_ENT))),
};

// Send Strings
char send_string_actions[][10] = {
    "\e:wq\n",
};

char send_string_fast_actions[][20] = {
    "xavv1m@gmail.com",
    "xavier@malparty.fr",
    "Malparty",
    "Xavier",
    "gttt ",
    "git push --force\n",
    "Thanks :pray: ",
    "Nice catch! ",
    ":white_check_mark: ",
    ":harold: ",
    "the",
    "and",
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t i = led_min; i < led_max; i++) {
            if (i%2 == 0) {
                rgb_matrix_set_color(i, RGB_RED);
            } else {
                rgb_matrix_set_color(i, RGB_BLUE);
            }
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // TAP DANCE
    tap_dance_action_t *action;

    switch (keycode) {
        // list all tap dance keycodes with tap-hold configurations
        case TD(TD_TAB_MOUSE):
        case TD(TD_SPC_NAV):
        case TD(TD_BSP_NUM):
        case TD(TD_ENT_SYM):
        case TD(TD_QUOTE_SENDS):
        case TD(TD_BROWSER):
        case TD(TD_GITFIGMA):
        case TD(TD_RECTANGLE):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_layer_t *tap_hold = (tap_dance_tap_hold_layer_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            return true;
    }

    // SEND STRINGS
    if (keycode == SS_GITCM) {
        if (record->event.pressed) {
            SEND_STRING("git cm ''" SS_TAP(X_LEFT));
        }
            return false;
    }
    if (keycode == SS_GITCMSCI) {
        if (record->event.pressed) {
            SEND_STRING("git cm '[skip ci] '" SS_TAP(X_LEFT));
        }
            return false;
    }
    else if (keycode == SS_E_GRAVE) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "`e");
        }
        return false;
    }
    else if (keycode == SS_A_GRAVE) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "`a");
        }
        return false;
    }
    else if (keycode == SS_U_GRAVE) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "`u");
        }
        return false;
    }
    else if (keycode == SS_E_AIGU) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "'e");
        }
        return false;
    }
    else if (keycode == SS_A_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "^a");
        }
        return false;
    }
    else if (keycode == SS_E_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "^e");
        }
        return false;
    }
    else if (keycode == SS_I_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "^i");
        }
        return false;
    }
    else if (keycode == SS_U_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "^u");
        }
        return false;
    }
    else if (keycode == SS_O_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) "^o");
        }
        return false;
    }
    else if (keycode == SS_C_CEDI) {
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_CAPS) ",c");
        }
        return false;
    }
    else if (keycode > SS_SLOW_START && keycode < SS_FAST_START) {
        if (record->event.pressed) {
            int action_index = keycode - SS_SLOW_START - 1;
            SEND_STRING_DELAY(send_string_actions[action_index], 25);
        }
        return false;
    }
    else if (keycode > SS_FAST_START && keycode < SS_END) {
        if (record->event.pressed) {
            int action_index = keycode - SS_FAST_START - 1;
            SEND_STRING(send_string_fast_actions[action_index]);
        }
        return false;
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT(
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
                     KC_ESC,    KC_W        ,       KC_F       ,       KC_P       ,      KC_B       ,           KC_J       ,      KC_L        ,       KC_U       ,       KC_Y       ,LT(_SENDSTR, KC_Q),
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
       MT(MOD_LCTL, KC_A),MT(MOD_LALT, KC_R),MT(MOD_LGUI, KC_S),MT(MOD_LSFT, KC_T),      KC_G       ,           KC_M       ,MT(MOD_RSFT, KC_N),MT(MOD_RGUI, KC_E),MT(MOD_RALT, KC_I),MT(MOD_RCTL, KC_O),
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
         LT(_MOUSE2, KC_Z),     KC_X        ,       KC_C       ,       KC_D       ,       KC_V      ,           KC_K       ,      KC_H        ,     KC_COMM      ,      KC_DOT      ,LT(_RGB, KC_SLSH) ,
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
    //                                           |-------------------------+-------------------------| |-------------------------+-------------------------|
                                                       LT(_MOUSE, KC_TAB)  ,        LT(_NAV, KC_SPC) ,     LT(_NUM, KC_BSPC)     ,    LT(_SYM, KC_ENTER)
    //                                           |-------------------------+-------------------------| |-------------------------+-------------------------|
    ),

	[_MOUSE] = LAYOUT(
            TD(TD_GITFIGMA), TD(TD_BROWSER), LGUI(KC_1), LGUI(KC_7), LGUI(KC_4),        KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
            KC_LCTL,         KC_LOPT,        KC_LGUI,    KC_LSFT,    LCTL(LSFT(KC_E)),  KC_NO,     MS_LEFT,    MS_DOWN,    MS_UP,      MS_RGHT,
            KC_LEFT,         KC_UP,          KC_DOWN,    KC_RGHT,    TD(TD_RECTANGLE),  MS_BTN3,   MS_WHLL,    MS_WHLU,    MS_WHLD,    MS_WHLR,
                                                         KC_TRNS,    KC_NO,             MS_BTN2,   MS_BTN1
        ),
        [_MOUSE2] = LAYOUT_split_3x6_3(
            KC_DEL,  KC_NO,    KC_F2,    KC_F12,   KC_F11,         KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,
            KC_LCTL, KC_LOPT,  KC_NO,    KC_NO,    KC_PSCR,        KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,
            KC_TRNS, KC_BTN6,  KC_BTN2,  KC_BTN1,  KC_NO,          KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,
                                      KC_TRNS, KC_TRNS,    KC_NO,         KC_BTN2,   KC_BTN1, KC_NO
        ),
	[_NAV] = LAYOUT(
        LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_Y),      KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_LCTL, KC_LOPT, KC_LGUI,  KC_LSFT, LCTL(KC_A),                 KC_NO, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        LCTL(LSFT(KC_T)), LCTL(KC_C), KC_BSPC, KC_ENT, QK_REP,           KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                                        KC_NO, KC_TRNS,                 KC_TRNS, KC_TRNS
        ),
	[_GAME] = LAYOUT(
        KC_NO, KC_Q, KC_W, KC_E, KC_R,                 KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO, KC_A, KC_S, KC_D, KC_F,                 KC_NO, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        KC_0, KC_1, KC_2, KC_3, KC_4,                  KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                                        KC_NO, KC_TRNS,                 KC_TRNS, KC_TRNS
        ),
	[_NUM] = LAYOUT(
            KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,                         KC_BRIU, SS_WQ,   KC_TRNS,           KC_TRNS,         KC_TRNS,
            KC_SCLN, KC_4, KC_5, KC_6, KC_EQL,                          KC_BRID, KC_TRNS, KC_TRNS,           KC_TRNS,         KC_TRNS,
            KC_GRV, KC_1, KC_2, KC_3, KC_BSLS,                          KC_MPRV, KC_MPLY, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_MNXT,
                                KC_0, KC_DOT,                           KC_TRNS, KC_NO
        ),
	[_SYM] = LAYOUT(
            KC_LCBR, KC_AMPR, KC_ASTR, KC_MINS, KC_RCBR,                TO(_GAME), TO(_GAME),   TO(_GAME), TO(_GAME), DF(_BASE),
            KC_COLN, KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,                 KC_NO, KC_LSFT, KC_LGUI,   KC_LALT,  KC_LCTL,
            KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_PIPE,                  KC_NO, KC_LEFT, KC_DOWN,   KC_UP,    KC_RGHT,
                                     KC_LPRN, KC_RPRN,                  KC_NO, KC_TRNS
        ),
	[_SENDSTR] = LAYOUT(
            SS_FNAME, SS_LNAME, SS_NC, SS_TX,   SS_GITCM,               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            SS_MAILP, KC_NO,    KC_NO, SS_CK,   SS_GITCMSCI,            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            SS_MAILW, KC_NO,    KC_NO, SS_HARO, KC_NO,                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                    SS_GTPF,    KC_NO,                  KC_TRNS, KC_TRNS
        ),
	[_RGB] = LAYOUT(
        RM_TOGG, RM_NEXT,  RM_HUEU, RM_SATU, RM_VALU,                   KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
        RM_SPDU, RM_PREV, RM_HUED, RM_SATD, RM_VALD,                   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        RM_SPDD, KC_NO,    KC_NO,   KC_NO,   KC_NO,                     KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                                    KC_NO,   KC_NO,                     TO(_BASE), TO(_BASE)
    )
    // // DUPLICATE ME
	// [_TEMPLATE] = LAYOUT(
    //         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    //         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    //         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    //                                    KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS
    //     ),
};
