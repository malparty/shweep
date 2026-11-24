// Copyright 2022 splitkb.com <support@splitkb.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* RGB matrix (underglow is part of the matrix too).
 * SOLID_REACTIVE_SIMPLE, SOLID_SPLASH, RAINBOW_BEACON and sleep are already
 * enabled by the Halcyon Ferris keyboard.json. */
#define RGB_MATRIX_KEYPRESSES

/* Sync CAPS LOCK info to both halves (for LED effect + display) */
#define SPLIT_LED_STATE_ENABLE

/* Display sheep: chew on key presses from both halves, and show typing speed */
#define SPLIT_ACTIVITY_ENABLE
#define SPLIT_WPM_ENABLE

/* Mouse */
// The default is 100
#define MOUSEKEY_WHEEL_INTERVAL 50
// The default is 40
#define MOUSEKEY_WHEEL_TIME_TO_MAX 100

/* Tap dance */
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 0

/* Halcyon encoder module */
#define ENCODER_RESOLUTION 2
