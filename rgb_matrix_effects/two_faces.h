RGB_MATRIX_EFFECT(TWO_FACES)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS


static bool TWO_FACES(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t  huedelta = 18;
    HSV      hsv      = rgb_matrix_config.hsv;
    uint16_t time     = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    hsv.h             = hsv.h + scale8(abs8(sin8(time) - 128) * 2, huedelta);
    RGB rgb           = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        if(i < 6 || (i > 22 && i < 29)) { // Underglow
            rgb_matrix_set_color(i, rgb.b, rgb.r, rgb.g);
        } else { // Keys
        if(i%2 == 0) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }else{
            rgb_matrix_set_color(i, rgb.r, rgb.b, rgb.g);
        }

        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
