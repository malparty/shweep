RGB_MATRIX_EFFECT(XAVV_RAINBOW_THUMBS)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static HSV XAVV_RAINBOW_THUMBS_math(HSV hsv, int8_t sin, int8_t cos, uint8_t i, uint8_t time) {
    if (g_led_config.point[i].y > 50) {
        hsv.h -= 60;
    } else { // Rainbow
        hsv.h += g_led_config.point[i].x % 128;
    }

    return hsv;
}

bool XAVV_RAINBOW_THUMBS(effect_params_t* params) {
    return effect_runner_sin_cos_i(params, &XAVV_RAINBOW_THUMBS_math);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
