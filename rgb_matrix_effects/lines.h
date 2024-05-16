RGB_MATRIX_EFFECT(LINES)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS


static bool LINES(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t  huedelta = 28;
    HSV      hsv      = rgb_matrix_config.hsv;
    uint16_t time     = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    hsv.h             = hsv.h + scale8(abs8(sin8(time) - 128) * 2, huedelta);
    RGB rgb           = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        if(i < 6 || (i > 22 && i < 29)) { // Underglow
            rgb_matrix_set_color(i, rgb.b, rgb.r, rgb.g);
        } else { // Keys
            if((i > 43) || (i > 20 && i < 23)) { // Thumbs
                rgb_matrix_set_color(i, rgb.b, rgb.r, rgb.g);
            }
            else if((i > 28 && (i + 1)%10 > 4) || (i < 21 && (i - 1)%10 > 4)) { // Line even
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }else{ // Line odd
                rgb_matrix_set_color(i, rgb.g, rgb.b, rgb.r);
            }
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
