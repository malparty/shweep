#include QMK_KEYBOARD_H
#include "keycodes.h"

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
