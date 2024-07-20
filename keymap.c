#include QMK_KEYBOARD_H
#include "keycodes.h"

char send_string_actions[][10] = {
    "\e:wq\n",
};

char send_string_fast_actions[][20] = {
    "xavier@nimblehq.co",
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
            SEND_STRING(SS_LOPT("`") "e");
        }
        return false;
    }
    else if (keycode == SS_A_GRAVE) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("`") "a");
        }
        return false;
    }
    else if (keycode == SS_U_GRAVE) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("`") "u");
        }
        return false;
    }
    else if (keycode == SS_E_AIGU) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("e") "e");
        }
        return false;
    }
    else if (keycode == SS_A_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("i") "a");
        }
        return false;
    }
    else if (keycode == SS_E_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("i") "e");
        }
        return false;
    }
    else if (keycode == SS_I_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("i") "i");
        }
        return false;
    }
    else if (keycode == SS_U_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("i") "u");
        }
        return false;
    }
    else if (keycode == SS_O_CIRCUMFLEX) {
        if (record->event.pressed) {
            SEND_STRING(SS_LOPT("i") "o");
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
       MT(MOD_LGUI, KC_A),MT(MOD_LALT, KC_R),MT(MOD_LCTL, KC_S),MT(MOD_LSFT, KC_T),      KC_G       ,           KC_M       ,MT(MOD_RSFT, KC_N),MT(MOD_RCTL, KC_E),MT(MOD_RALT, KC_I),MT(MOD_RGUI, KC_O),
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
         LT(_MOUSE2, KC_Z),     KC_X        ,       KC_C       ,       KC_D       ,       KC_V      ,           KC_K       ,      KC_H        ,     KC_COMM      ,      KC_DOT      ,LT(_RGB, KC_SLSH) ,
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
    //                                           |-------------------------+-------------------------| |-------------------------+-------------------------|
                                                       LT(_MOUSE, KC_TAB)  ,        LT(_NAV, KC_SPC) ,     LT(_NUM, KC_BSPC)     ,    LT(_SYM, KC_ENTER)
    //                                           |-------------------------+-------------------------| |-------------------------+-------------------------|
    ),

    ////////////////////////////////////////////////
    //////////     WINDOWS   LAYOUTS      //////////
    ////////////////////////////////////////////////
	[_BASEWIN] = LAYOUT(
        KC_TRNS,           KC_TRNS, KC_TRNS,            KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS,
        MT(MOD_LCTL, KC_A),KC_TRNS, MT(MOD_LGUI, KC_S), KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, MT(MOD_RGUI, KC_E),KC_TRNS, MT(MOD_RCTL, KC_O),
        KC_TRNS,           KC_TRNS, KC_TRNS,            KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS,
                                    LT(_MOUSE, KC_TAB), LT(_NAVWIN, KC_SPC),         LT(_NUM, KC_BSPC), LT(_SYM, KC_ENTER)
    ),
	[_MOUSE] = LAYOUT(
            TD(TD_GITFIGMA), TD(TD_BROWSER), LGUI(KC_1), LGUI(KC_7), LGUI(KC_4),        KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
            KC_LGUI,         KC_LOPT,        KC_LCTL,    KC_LSFT,    LGUI(LSFT(KC_E)),  KC_NO,     KC_MS_L,    KC_MS_D,    KC_MS_U,    KC_MS_R,
            KC_LEFT,         KC_UP,          KC_DOWN,    KC_RGHT,    TD(TD_RECTANGLE),  KC_BTN3,   KC_WH_L,    KC_WH_U,    KC_WH_D,    KC_WH_R,
                                                         KC_TRNS,    KC_NO,             KC_BTN2,   KC_BTN1
        ),
	[_MOUSE2] = LAYOUT(
            KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,
            KC_LGUI, KC_LOPT, KC_LCTL, KC_LSFT, KC_NO,      KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,
            KC_TRNS, KC_BTN4, KC_BTN2, KC_BTN1, KC_NO,      KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,
                                       KC_TRNS, KC_NO,      KC_BTN2, KC_BTN1
        ),
	[_NAV] = LAYOUT(
        LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), RSG(KC_Z),      KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_LGUI, KC_LOPT, KC_LCTL, KC_LSFT, LGUI(KC_A),                 KC_NO, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        LGUI(LSFT(KC_T)), LCTL(KC_C), KC_BSPC, KC_ENT, KC_NO,           KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                                        KC_NO, KC_TRNS,                 KC_TRNS, KC_TRNS
        ),
	[_NAVWIN] = LAYOUT(
        LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_Y),     KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_LCTL, KC_LOPT, KC_LGUI, KC_LSFT, LCTL(KC_A),                 KC_NO, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        LCTL(LSFT(KC_T)), LCTL(KC_C), KC_BSPC, KC_ENT, KC_NO,           KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                                        KC_NO, KC_TRNS,                 KC_TRNS, KC_TRNS
        ),
	[_NUM] = LAYOUT(
            KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,                         KC_BRIU, SS_WQ,   KC_TRNS,           KC_TRNS,         KC_TRNS,
            KC_SCLN, KC_4, KC_5, KC_6, KC_EQL,                          KC_BRID, KC_TRNS, KC_TRNS,           KC_TRNS,         KC_TRNS,
            KC_GRV, KC_1, KC_2, KC_3, KC_BSLS,                          KC_MPRV, KC_MPLY, KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_MNXT,
                                KC_0, KC_DOT,                           KC_TRNS, KC_NO
        ),
	[_SYM] = LAYOUT(
            KC_LCBR, KC_AMPR, KC_ASTR, KC_MINS, KC_RCBR,                KC_NO, KC_NO,   TO(_GAME), DF(_BASEWIN), DF(_BASE),
            KC_COLN, KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,                 KC_NO, KC_LSFT, KC_LCTL,   KC_LALT,  KC_LGUI,
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
        RGB_TOG, RGB_MOD,  RGB_HUI, RGB_SAI, RGB_VAI,                   KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
        RGB_SPI, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD,                   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        RGB_SPD, KC_NO,    KC_NO,   KC_NO,   KC_NO,                     KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                                    KC_NO,   KC_NO,                     TO(_BASE), TO(_BASE)
    ),
    [_GAME] = LAYOUT(
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
                     KC_Q,    KC_W          ,       KC_E       ,       KC_R       ,      KC_T       ,           KC_9       ,      KC_P        ,       KC_U       ,       KC_Y       ,    TO(_BASE)     ,
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
                     KC_A,    KC_S          ,       KC_D        ,      KC_F       ,      KC_G       ,           KC_4       ,      KC_5        ,       KC_6       ,       KC_7       ,     KC_8         ,
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
                     KC_Z,    KC_X          ,       KC_C       ,       KC_V       ,       KC_B      ,           KC_2       ,      KC_3        ,     KC_COMM      ,      KC_DOT      ,     KC_SLSH      ,
    //|------------------+------------------+------------------+------------------+-----------------|   |------------------+------------------+------------------+------------------+------------------|
    //                                           |-------------------------+-------------------------| |-------------------------+-------------------------|
                                                         KC_L              ,        KC_N             ,         KC_0          ,    KC_1
    //                                           |-------------------------+-------------------------| |-------------------------+-------------------------|
    )
    // // DUPLICATE ME
	// [_TEMPLATE] = LAYOUT(
    //         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    //         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    //         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    //                                    KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS
    //     ),
};
