// SPDX-License-Identifier: GPL-2.0-or-later
// Right-half TFT (135x240): current layer, Shweep the sheep, typing speed and active modifiers.
//
// The sheep chews on every key press (from both halves, thanks to SPLIT_ACTIVITY_ENABLE),
// chews faster as the WPM rises, blinks now and then, and falls asleep when idle.

#include QMK_KEYBOARD_H
#include <stdlib.h>
#include "keycodes.h"

#if defined(HLC_TFT_DISPLAY)
#    include "hlc_tft_display/hlc_tft_display.h"
#    include "hlc_tft_display/graphics/fonts/Retron2000-27.qff.h"
#    include "hlc_tft_display/graphics/fonts/Retron2000-underline-27.qff.h"
#    include "sheep.h"

// Layout (y positions, the screen is 135 x 240).
#    define LAYER_NAME_Y 20
#    define SHEEP_X ((LCD_WIDTH - SHEEP_W * SHEEP_SCALE) / 2)
#    define SHEEP_Y 54
#    define GRASS_Y (SHEEP_Y + SHEEP_H * SHEEP_SCALE)
#    define WPM_Y 162

// Animation timings (ms).
#    define FRAME_INTERVAL 30
#    define SLEEP_AFTER 30000
#    define CHEW_MIN 60  // mouth-open time per key press at high WPM
#    define CHEW_MAX 160 // ... and at low WPM
#    define BLINK_TIME 140
#    define BLINK_MIN_GAP 2000
#    define BLINK_MAX_GAP 6000
#    define ZZZ_STEP 600

static painter_font_handle_t font;
static painter_font_handle_t font_ul;

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

// ---------------------------------------------------------------- Sheep ---

static void draw_sheep(bool eyes_open, bool mouth_open) {
    qp_rect(lcd_surface, SHEEP_X, SHEEP_Y, SHEEP_X + SHEEP_W * SHEEP_SCALE - 1, GRASS_Y - 1, HSV_BLACK, true);
    for (uint8_t y = 0; y < SHEEP_H; y++) {
        for (uint8_t x = 0; x < SHEEP_W; x++) {
            hsv_t c;
            if (sheep_color(sheep_art[y][x], eyes_open, mouth_open, &c)) {
                uint16_t px = SHEEP_X + x * SHEEP_SCALE;
                uint16_t py = SHEEP_Y + y * SHEEP_SCALE;
                qp_rect(lcd_surface, px, py, px + SHEEP_SCALE - 1, py + SHEEP_SCALE - 1, c.h, c.s, c.v, true);
            }
        }
    }
}

static void draw_grass(void) {
    // Meadow strip with a few blades poking up.
    qp_rect(lcd_surface, 0, GRASS_Y, LCD_WIDTH - 1, GRASS_Y + 5, 85, 255, 120, true);
    for (uint16_t x = 2; x < LCD_WIDTH - 2; x += 7) {
        uint8_t h = (x * 5) % 4 + 2;
        qp_rect(lcd_surface, x, GRASS_Y - h, x + 1, GRASS_Y - 1, 85, 255, 160, true);
    }
}

// A small "z" drawn from a 4x4 bitmap.
static void draw_z(uint16_t x, uint16_t y, uint8_t scale, bool visible) {
    static const uint8_t z_bits[4] = {0b1111, 0b0010, 0b0100, 0b1111};
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            bool    on = visible && (z_bits[row] & (0b1000 >> col));
            uint8_t v  = on ? 230 : 0;
            qp_rect(lcd_surface, x + col * scale, y + row * scale, x + (col + 1) * scale - 1, y + (row + 1) * scale - 1, 150, on ? 80 : 0, v, true);
        }
    }
}

// zZz rising from the sleeping sheep's head: step 0 = none, 1..3 = that many z's.
static void draw_zzz(uint8_t step) {
    draw_z(66, 64, 2, step >= 1);
    draw_z(50, 58, 3, step >= 2);
    draw_z(30, 50, 4, step >= 3);
}

static void draw_wpm(uint8_t wpm) {
    char buf[12] = "";
    if (wpm > 0) {
        snprintf(buf, sizeof(buf), "%u wpm", wpm);
    }
    draw_centered(WPM_Y, font, buf, 0, 0, 140);
}

// --------------------------------------------------------- Housekeeping ---

bool display_module_housekeeping_task_user(bool second_display) {
    if (second_display) {
        return true; // Only one display on this board; keep splitkb's default just in case.
    }

    static bool     init       = false;
    static uint8_t  last_layer = 0xFF;
    static uint8_t  last_mods  = 0xFF;
    static bool     dirty      = true;
    static uint32_t last_frame = 0;

    // Sheep state.
    static uint32_t seen_activity = 0;
    static uint32_t mouth_until   = 0; // local timer
    static uint32_t next_blink    = 0;
    static uint32_t blink_until   = 0;
    static int8_t   drawn_eyes    = -1;
    static int8_t   drawn_mouth   = -1;
    static int8_t   drawn_zzz     = -1;
    static int16_t  drawn_wpm     = -1;

    uint32_t now = timer_read32();

    if (!init) {
        font    = qp_load_font_mem(font_Retron2000_27);
        font_ul = qp_load_font_mem(font_Retron2000_underline_27);
        srand(now);
        next_blink    = now + BLINK_MIN_GAP;
        seen_activity = last_input_activity_time();
        init = true;
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
        dirty      = true;
    }

    if (timer_elapsed32(last_frame) >= FRAME_INTERVAL) {
        last_frame = now;

        uint8_t wpm    = get_current_wpm();
        bool    asleep = last_input_activity_elapsed() > SLEEP_AFTER;

        // Chew: every new key press / release / encoder turn opens the mouth for a moment,
        // shorter when typing fast so the sheep munches quicker.
        if (last_input_activity_time() != seen_activity) {
            seen_activity      = last_input_activity_time();
            uint8_t  speed     = wpm > 100 ? 100 : wpm;
            uint32_t chew_time = CHEW_MAX - (uint32_t)(CHEW_MAX - CHEW_MIN) * speed / 100;
            mouth_until        = now + chew_time;
        }
        bool mouth_open = !asleep && !timer_expired32(now, mouth_until);

        // Blink at random intervals.
        if (timer_expired32(now, next_blink)) {
            blink_until = now + BLINK_TIME;
            next_blink  = now + BLINK_MIN_GAP + rand() % (BLINK_MAX_GAP - BLINK_MIN_GAP);
        }
        bool eyes_open = !asleep && !timer_expired32(now, blink_until);

        uint8_t zzz = asleep ? (now / ZZZ_STEP) % 4 : 0;

        if (eyes_open != drawn_eyes || mouth_open != drawn_mouth || dirty) {
            draw_sheep(eyes_open, mouth_open);
            draw_grass(); // blades overlap the sheep's feet
            drawn_eyes  = eyes_open;
            drawn_mouth = mouth_open;
        }
        if (zzz != drawn_zzz || dirty) {
            draw_zzz(zzz);
            drawn_zzz = zzz;
        }
        if (wpm != drawn_wpm || dirty) {
            draw_wpm(wpm);
            drawn_wpm = wpm;
        }
    }

    uint8_t mods = get_mods() | get_oneshot_mods();
    if (mods != last_mods || dirty) {
        static const struct {
            uint8_t     mask;
            const char *label;
        } mod_labels[] = {
            {MOD_MASK_CTRL, "C"},
            {MOD_MASK_ALT, "A"},
            {MOD_MASK_GUI, "G"},
            {MOD_MASK_SHIFT, "S"},
        };
        int16_t y = LCD_HEIGHT - font->line_height - 10;
        qp_rect(lcd_surface, 0, y, LCD_WIDTH - 1, y + font->line_height, HSV_BLACK, true);
        int16_t slot = LCD_WIDTH / 4;
        for (uint8_t i = 0; i < ARRAY_SIZE(mod_labels); i++) {
            bool    on = mods & mod_labels[i].mask;
            int16_t w  = qp_textwidth(font, mod_labels[i].label);
            int16_t x  = slot * i + (slot - w) / 2;
            if (on) {
                qp_drawtext_recolor(lcd_surface, x, y, font_ul, mod_labels[i].label, HSV_SPLITKB, HSV_BLACK);
            } else {
                qp_drawtext_recolor(lcd_surface, x, y, font, mod_labels[i].label, 0, 0, 60, HSV_BLACK);
            }
        }
        last_mods = mods;
    }

    dirty = false;

    qp_surface_draw(lcd_surface, lcd, 0, 0, 0);
    qp_flush(lcd);
    return false; // Skip splitkb's default layer-number screen.
}
#endif
