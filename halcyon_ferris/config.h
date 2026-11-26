// Copyright 2022 splitkb.com <support@splitkb.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* RGB matrix (underglow is part of the matrix too).
 * SOLID_REACTIVE_SIMPLE, SOLID_SPLASH, RAINBOW_BEACON and sleep are already
 * enabled by the Halcyon Ferris keyboard.json. */
#define RGB_MATRIX_KEYPRESSES

/* Sync CAPS LOCK info to both halves (for LED effect + display) */
#define SPLIT_LED_STATE_ENABLE

/* Display pet: chew on key presses from both halves */
#define SPLIT_ACTIVITY_ENABLE

/* Mouse */
// The default is 100
#define MOUSEKEY_WHEEL_INTERVAL 50
// The default is 40
#define MOUSEKEY_WHEEL_TIME_TO_MAX 100

/* Tap dance */
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 0

/* Home row mods: fewer accidental mods (= "eaten" letters) on fast rolls.
 * Flow Tap: a home-row key pressed < FLOW_TAP_TERM ms after the previous
 *   letter is always a tap, so mid-word rolls never become a mod.
 * Chordal Hold: a home-row key followed by a key on the SAME hand is a tap
 *   (mods are for the other hand); thumbs are exempt, see chordal_hold_layout.
 * Permissive Hold: opposite-hand nested press (mod down, key down+up, mod up)
 *   is a hold right away, without waiting out TAPPING_TERM. */
#define FLOW_TAP_TERM 150
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

/* Halcyon encoder module */
#define ENCODER_RESOLUTION 2
