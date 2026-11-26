// SPDX-License-Identifier: GPL-2.0-or-later
// Right-half TFT (135x240): current layer name, then Shweep the sheep in its meadow.
//
// The pet chews on every key press (from both halves, thanks to SPLIT_ACTIVITY_ENABLE),
// blinks now and then, falls asleep when idle, and dresses up with the active modifiers
// (see pets.h).

#include QMK_KEYBOARD_H
#include <stdlib.h>
#include "keycodes.h"

#if defined(HLC_TFT_DISPLAY)
#    include "hlc_tft_display/hlc_tft_display.h"
#    include "hlc_tft_display/graphics/fonts/Retron2000-27.qff.h"
#    include "pets.h"

// Layout (y positions, the screen is 135 x 240).
#    define LAYER_NAME_Y 20
#    define SCENE_TOP 50
#    define SCENE_GROUND 228

// Animation timings (ms).
#    define FRAME_INTERVAL 30
#    define SLEEP_AFTER 30000
#    define CHEW_TIME 110 // mouth-open time per key press
#    define BLINK_TIME 140
#    define BLINK_MIN_GAP 2000
#    define BLINK_MAX_GAP 6000
#    define ZZZ_STEP 600
#    define CLOUD_STEP 150 // ms per pixel of cloud drift

static painter_font_handle_t font;

typedef struct {
    const char *name;
    uint8_t     h, s, v;
} layer_style_t;

static const layer_style_t layer_styles[] = {
    [_BASE]    = {"BASE", 0, 0, 200},
    [_MOUSE]   = {"MOUSE", 23, 200, 255},
    [_MOUSE2]  = {"MOUSE2", 43, 200, 255},
    [_NAV]     = {"NAV", 85, 200, 255},
    [_NUM]     = {"NUM", 131, 200, 255},
    [_SYM]     = {"SYM", 176, 200, 255},
    [_SENDSTR] = {"SEND", 213, 200, 255},
    [_RGB]     = {"RGB", 0, 255, 255},
    [_GAME]    = {"GAME", 10, 255, 255},
};

static void draw_centered(int16_t y, painter_font_handle_t f, const char *text, uint8_t h, uint8_t s, uint8_t v) {
    // Clear the full line first so shorter strings don't leave leftovers.
    qp_rect(lcd_surface, 0, y, LCD_WIDTH - 1, y + f->line_height, HSV_BLACK, true);
    int16_t w = qp_textwidth(f, text);
    int16_t x = (LCD_WIDTH - w) / 2;
    qp_drawtext_recolor(lcd_surface, x < 0 ? 0 : x, y, f, text, h, s, v, HSV_BLACK);
}

// Scene rectangles, clipped to the scene area.
static void scene_rect(int16_t x, int16_t y, int16_t w, int16_t h, hsv_t c) {
    int16_t x1 = x + w - 1, y1 = y + h - 1;
    if (x < 0) x = 0;
    if (y < SCENE_TOP) y = SCENE_TOP;
    if (x1 > LCD_WIDTH - 1) x1 = LCD_WIDTH - 1;
    if (y1 > LCD_HEIGHT - 1) y1 = LCD_HEIGHT - 1;
    if (x > x1 || y > y1) return;
    qp_rect(lcd_surface, x, y, x1, y1, c.h, c.s, c.v, true);
}

bool display_module_housekeeping_task_user(bool second_display) {
    if (second_display) {
        return true; // Only one display on this board; keep splitkb's default just in case.
    }

    static bool     init       = false;
    static uint8_t  last_layer = 0xFF;
    static uint32_t last_frame = 0;

    static uint32_t seen_activity = 0;
    static uint32_t mouth_until   = 0;
    static uint32_t next_blink    = 0;
    static uint32_t blink_until   = 0;
    static scene_t  drawn;
    static bool     scene_drawn = false;

    uint32_t now = timer_read32();

    if (!init) {
        font = qp_load_font_mem(font_Retron2000_27);
        srand(now);
        next_blink    = now + BLINK_MIN_GAP;
        seen_activity = last_input_activity_time();
        init          = true;
    }

    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    if (layer != last_layer) {
        const layer_style_t *st = layer < ARRAY_SIZE(layer_styles) && layer_styles[layer].name ? &layer_styles[layer] : NULL;
        // Coloured bar + name at the top.
        if (st) {
            qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, 6, st->h, st->s, st->v, true);
            draw_centered(LAYER_NAME_Y, font, st->name, st->h, st->s, st->v);
        } else {
            qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, 6, HSV_RED, true);
            draw_centered(LAYER_NAME_Y, font, "???", HSV_RED);
        }
        last_layer = layer;
    }

    if (timer_elapsed32(last_frame) >= FRAME_INTERVAL) {
        last_frame = now;

        bool asleep = last_input_activity_elapsed() > SLEEP_AFTER;

        // Chew: every key press / release / encoder turn opens the mouth for a moment.
        if (last_input_activity_time() != seen_activity) {
            seen_activity = last_input_activity_time();
            mouth_until   = now + CHEW_TIME;
        }

        // Blink at random intervals.
        if (timer_expired32(now, next_blink)) {
            blink_until = now + BLINK_TIME;
            next_blink  = now + BLINK_MIN_GAP + rand() % (BLINK_MAX_GAP - BLINK_MIN_GAP);
        }

        uint8_t mods = get_mods() | get_oneshot_mods();
        scene_t st;
        memset(&st, 0, sizeof(st)); // zero the padding too, the struct is compared with memcmp
        st.eyes_open  = !asleep && timer_expired32(now, blink_until);
        st.mouth_open = !asleep && !timer_expired32(now, mouth_until);
        st.zzz_step   = asleep ? (now / ZZZ_STEP) % 4 : 0;
        st.chicken    = mods & MOD_MASK_ALT;
        st.long_legs  = mods & MOD_MASK_SHIFT;
        st.super      = mods & MOD_MASK_GUI;
        st.police     = mods & MOD_MASK_CTRL;
        // Wraps after a whole number of laps for every cloud speed (1, 2 and 3), so no jump.
        st.cloud_offset = (now / CLOUD_STEP) % ((LCD_WIDTH + SCENE_CLOUD_W) * 6);

        if (!scene_drawn || memcmp(&st, &drawn, sizeof(st)) != 0) {
            scene_draw(scene_rect, &st, LCD_WIDTH, SCENE_TOP, SCENE_GROUND);
            drawn       = st;
            scene_drawn = true;
        }
    }

    qp_surface_draw(lcd_surface, lcd, 0, 0, 0);
    qp_flush(lcd);
    return false; // Skip splitkb's default layer-number screen.
}
#endif
