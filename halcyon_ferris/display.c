// SPDX-License-Identifier: GPL-2.0-or-later
// Right-half TFT (135x240): current layer name, caps lock and active modifiers.

#include QMK_KEYBOARD_H
#include "keycodes.h"

#if defined(HLC_TFT_DISPLAY)
#    include "hlc_tft_display/hlc_tft_display.h"
#    include "hlc_tft_display/graphics/fonts/Retron2000-27.qff.h"
#    include "hlc_tft_display/graphics/fonts/Retron2000-underline-27.qff.h"

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

bool display_module_housekeeping_task_user(bool second_display) {
    if (second_display) {
        return true; // Only one display on this board; keep splitkb's default just in case.
    }

    static bool          init       = false;
    static uint8_t       last_layer = 0xFF;
    static led_t         last_leds  = {0};
    static uint8_t       last_mods  = 0xFF;
    static bool          dirty      = true;

    if (!init) {
        font    = qp_load_font_mem(font_Retron2000_27);
        font_ul = qp_load_font_mem(font_Retron2000_underline_27);
        init    = true;
    }

    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    if (layer != last_layer) {
        const layer_style_t *st = layer < ARRAY_SIZE(layer_styles) && layer_styles[layer].name ? &layer_styles[layer] : NULL;
        // Coloured bar + name at the top.
        if (st) {
            qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, 6, st->h, st->s, st->v, true);
            draw_centered(20, font, st->name, st->h, st->s, st->v);
        } else {
            qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, 6, HSV_RED, true);
            draw_centered(20, font, "???", HSV_RED);
        }
        last_layer = layer;
        dirty      = true;
    }

    led_t leds = host_keyboard_led_state();
    if (leds.raw != last_leds.raw || dirty) {
        if (leds.caps_lock) {
            draw_centered(110, font_ul, "CAPS", HSV_CAPS_ON);
        } else {
            draw_centered(110, font, "caps", HSV_CAPS_OFF);
        }
        last_leds = leds;
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
