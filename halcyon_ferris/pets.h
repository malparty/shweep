// SPDX-License-Identifier: GPL-2.0-or-later
// Shweep the sheep (and friends): pixel-art scene for the right-half TFT.
//
// The scene is stacked from small ASCII sprites, one character per pixel
// (drawn PET_SCALE x PET_SCALE on screen). Modifiers dress the pet up:
//   ALT   -> the sheep turns into a chicken (the alternative pet)
//   SHIFT -> extra long legs, the pet is shifted up into the clouds
//   GUI   -> Super(man) suit: blue body, red cape, "S" shield
//   CTRL  -> police hat (police control!)
// Every combination works because each piece is drawn on top of the previous one.
//
// Palette characters:
//   .  transparent
//   W  wool / feathers (blue in the Super suit)   w  their shadow
//   F  face       f  face shadow     k  ear       i  inner ear
//   o  pupil top  e  eye glint (both closed-eye colour when blinking or asleep)
//   p  pupil bottom / closed eyelid
//   r  nose       n  mouth line
//   m  open mouth (closed-mouth colour when closed)
//   G  grass over the face (closed-mouth colour when closed)
//   g  grass outside the face, b  lower beak (both hidden when the mouth is closed)
//   L  leg        H  hoof          R  red (comb, wattle, shield)   Y  yellow
//   C  cape       c  cape fold     B  police hat  t  hat band   K  visor
//   Q  cloud      q  cloud shadow

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "color.h"

#define PET_SCALE 4
#define PET_LEGS_NORMAL 3
#define PET_LEGS_LONG 18
#define SCENE_CLOUD_W (12 * PET_SCALE)

// clang-format off

// ------------------------------------------------------------- Sprites ---

static const char *const sheep_rows[] = {
    "................................",
    "........................W.W.....",
    ".......................WWWWW....",
    "....................kkWWWWWWWkk.",
    "...................kiiFFFFFFFiik",
    "...WWW..WWW..WWW....kkFFFFFFFkk.",
    "..WWWWWWWWWWWWWWWWWWWWFoeFoeF...",
    ".WWWWWWWWWWWWWWWWWWWWWFppFppF...",
    ".WWWWwWWWWWWwWWWWWWwWWFFFFFFF...",
    "WWWwWWWWwWWWWWWwWWWWWWFFFrFFF...",
    "WWWWWWWWWWWWWWWWWWWWWWFFnnnFFg..",
    "WWWWWwWWWWWwWWWWwWWWWWfFmmmGGg..",
    "wWWWWWWWWWWWWWWWWWWWWW.fFmFfg...",
    "wWWwWWWWWwWWWWWWwWWWWW..........",
    ".wWWWWWWWWWWWWWWWWWWWw..........",
    ".wwWWWWWWWWWWWWWWWWWww..........",
    "..wwwWWWwwwWWWWwwwWww...........",
    "...wwwwwwwwwwwwwwwwww...........",
};

static const char *const chicken_rows[] = {
    "..............R.R.....",
    ".............RRRRR....",
    "............WWWWWW....",
    "...........WWWWWWWW...",
    "W..........WWWWWoeW...",
    "WW.........WWWWWppWY..",
    "WWW........WWWWWWWWYYY",
    "WWWW.......WWWWWWWRmm.",
    "WWWWW.....WWWWWWWWRbb.",
    "WWWWWWWWWWWWWWWWWWR...",
    ".WWWWWWWWWWWWWWWWW....",
    ".WWWWwwwwWWWWWWWWW....",
    ".WWWWWwwwwwWWWWWWW....",
    "..WWWWWwwwwwWWWWW.....",
    "...WWWWWWWWWWWWWW.....",
    "....wWWWWWWWWWWWw.....",
    ".....wwwwwwwwwww......",
};

static const char *const police_hat_rows[] = {
    "..BBBBBBB..",
    ".BBBBYBBBB.",
    ".BBBBBBBBB.",
    "..ttttttt..",
    "..KKKKKKKKK",
};

static const char *const cape_rows[] = {
    "cCC...............",
    ".CCCCC.....cCC....",
    "..cCCCCCCCCCCCCCC.",
    "....CCCCCCCCCCCCCC",
    "........ccCCCCCCCC",
};

static const char *const shield_rows[] = {
    ".RRRRRRR.",
    "RYRRRRRYR",
    "RYRYYYYYR",
    ".RYRRRYR.",
    "..RYYRR..",
    "...RRR...",
};

static const char *const cloud_rows[] = {
    "....QQQ.....",
    "..QQQQQQ.QQ.",
    ".QQQQQQQQQQQ",
    "QQQQQQQQQQQQ",
    ".qqqqqqqqqq.",
};

// clang-format on

#define SPRITE_H(rows) ((int8_t)(sizeof(rows) / sizeof((rows)[0])))

// ---------------------------------------------------------------- Pets ---

typedef struct {
    const char *const *rows;
    int8_t             h;
    uint8_t            legs, leg_x[4], leg_w;
    char               leg_c;
    const char        *foot;
    int8_t             foot_dx;
    int8_t             hat_x, hat_y;       // police hat, in sprite pixels from the body's top-left
    int8_t             cape_x, cape_y;     // cape (drawn behind the body)
    int8_t             shield_x, shield_y; // "S" shield
    int16_t            zzz[3][3];          // x, y (screen pixels from the body's top-left), scale
    char               face_c, mouth_c;    // colours used for closed eyes / closed mouth
} pet_t;

static const pet_t pet_sheep = {
    .rows = sheep_rows, .h = SPRITE_H(sheep_rows),
    .legs = 4, .leg_x = {4, 8, 16, 20}, .leg_w = 2, .leg_c = 'L', .foot = "HH", .foot_dx = 0,
    .hat_x = 20, .hat_y = -2,
    .cape_x = 3, .cape_y = 1,
    .shield_x = 7, .shield_y = 8,
    .zzz = {{63, 10, 2}, {47, 4, 3}, {27, -4, 4}},
    .face_c = 'F', .mouth_c = 'F',
};

static const pet_t pet_chicken = {
    .rows = chicken_rows, .h = SPRITE_H(chicken_rows),
    .legs = 2, .leg_x = {8, 12}, .leg_w = 1, .leg_c = 'Y', .foot = "YYY", .foot_dx = -1,
    .hat_x = 10, .hat_y = -3,
    .cape_x = -4, .cape_y = 4,
    .shield_x = 4, .shield_y = 9,
    .zzz = {{40, -2, 2}, {26, -10, 3}, {8, -20, 4}},
    .face_c = 'W', .mouth_c = 'Y',
};

// --------------------------------------------------------------- Scene ---

typedef struct {
    bool    eyes_open, mouth_open;
    uint8_t zzz_step; // 0 = awake / no z, 1..3 = that many z's
    bool    chicken, long_legs, super, police;
    int16_t cloud_offset; // drift, in pixels
} scene_t;

// Fills a rectangle (screen pixels). The implementation must clip to the screen.
typedef void (*scene_rect_fn)(int16_t x, int16_t y, int16_t w, int16_t h, hsv_t c);

static inline bool pet_color(char c, const scene_t *st, const pet_t *pet, hsv_t *out) {
    switch (c) {
        case 'e':
        case 'o': if (!st->eyes_open) c = pet->face_c; break;
        case 'm':
        case 'G': if (!st->mouth_open) c = pet->mouth_c; break;
        case 'g':
        case 'b': if (!st->mouth_open) return false; break;
        default: break;
    }
    switch (c) {
        case 'W': *out = st->super ? (hsv_t){150, 170, 255} : (hsv_t){0, 0, 255}; return true;
        case 'w': *out = st->super ? (hsv_t){155, 200, 170} : (hsv_t){170, 25, 170}; return true;
        case 'F': *out = (hsv_t){28, 70, 235}; return true;
        case 'f': *out = (hsv_t){24, 90, 190}; return true;
        case 'k': *out = (hsv_t){22, 110, 170}; return true;
        case 'i': *out = (hsv_t){242, 100, 235}; return true;
        case 'e': *out = (hsv_t){0, 0, 255}; return true;
        case 'o':
        case 'p': *out = (hsv_t){0, 0, 15}; return true;
        case 'r': *out = (hsv_t){245, 150, 230}; return true;
        case 'n': *out = (hsv_t){10, 160, 90}; return true;
        case 'm': *out = (hsv_t){250, 210, 150}; return true;
        case 'G':
        case 'g': *out = (hsv_t){85, 255, 200}; return true;
        case 'L': *out = (hsv_t){20, 100, 120}; return true;
        case 'H': *out = (hsv_t){0, 0, 75}; return true;
        case 'R': *out = (hsv_t){0, 230, 230}; return true;
        case 'b':
        case 'Y': *out = (hsv_t){35, 230, 255}; return true;
        case 'C': *out = (hsv_t){0, 240, 220}; return true;
        case 'c': *out = (hsv_t){0, 240, 140}; return true;
        case 'B': *out = (hsv_t){165, 230, 150}; return true;
        case 't': *out = (hsv_t){165, 230, 70}; return true;
        case 'K': *out = (hsv_t){0, 0, 40}; return true;
        case 'Q': *out = (hsv_t){150, 45, 150}; return true;
        case 'q': *out = (hsv_t){150, 55, 100}; return true;
        default: return false;
    }
}

static inline void scene_sprite(scene_rect_fn rect, const char *const *rows, int8_t h, int16_t x0, int16_t y0, uint8_t scale, const scene_t *st, const pet_t *pet) {
    for (int8_t y = 0; y < h; y++) {
        for (int8_t x = 0; rows[y][x]; x++) {
            hsv_t c;
            if (pet_color(rows[y][x], st, pet, &c)) {
                rect(x0 + x * scale, y0 + y * scale, scale, scale, c);
            }
        }
    }
}

static inline void scene_z(scene_rect_fn rect, int16_t x, int16_t y, uint8_t scale) {
    static const uint8_t z_bits[4] = {0xF, 0x2, 0x4, 0xF};
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (z_bits[row] & (0x8 >> col)) {
                rect(x + col * scale, y + row * scale, scale, scale, (hsv_t){150, 80, 230});
            }
        }
    }
}

// Draws the whole scene between top and ground (screen pixels); width is the screen width.
static inline void scene_draw(scene_rect_fn rect, const scene_t *st, int16_t width, int16_t top, int16_t ground) {
    const pet_t *pet = st->chicken ? &pet_chicken : &pet_sheep;

    // Sky.
    rect(0, top, width, ground - top + 12, (hsv_t){0, 0, 0});

    // Drifting clouds (each at its own speed, wrapping around).
    static const int16_t clouds[][3] = {{0, 8, 1}, {70, 30, 2}, {20, 54, 3}}; // x, y from top, slowness
    const int16_t        cloud_w     = SCENE_CLOUD_W;
    for (uint8_t i = 0; i < sizeof(clouds) / sizeof(clouds[0]); i++) {
        int16_t span = width + cloud_w;
        int16_t x    = (clouds[i][0] + st->cloud_offset / clouds[i][2]) % span - cloud_w;
        scene_sprite(rect, cloud_rows, SPRITE_H(cloud_rows), x, top + clouds[i][1], PET_SCALE, st, pet);
    }

    // Pet position: feet on the ground, centred.
    int16_t pet_w    = (int16_t)strlen(pet->rows[0]);
    int8_t  leg_rows = st->long_legs ? PET_LEGS_LONG : PET_LEGS_NORMAL;
    int16_t x0       = (width - pet_w * PET_SCALE) / 2;
    int16_t y0       = ground - (pet->h + leg_rows + 1) * PET_SCALE;
#define AT(px, py) x0 + (px) * PET_SCALE, y0 + (py) * PET_SCALE

    if (st->super) {
        scene_sprite(rect, cape_rows, SPRITE_H(cape_rows), AT(pet->cape_x, pet->cape_y), PET_SCALE, st, pet);
    }

    // Legs and feet.
    hsv_t leg_c, foot_c;
    pet_color(pet->leg_c, st, pet, &leg_c);
    pet_color(pet->foot[0], st, pet, &foot_c);
    for (uint8_t i = 0; i < pet->legs; i++) {
        rect(AT(pet->leg_x[i], pet->h), pet->leg_w * PET_SCALE, leg_rows * PET_SCALE, leg_c);
        rect(AT(pet->leg_x[i] + pet->foot_dx, pet->h + leg_rows), (int16_t)strlen(pet->foot) * PET_SCALE, PET_SCALE, foot_c);
    }

    scene_sprite(rect, pet->rows, pet->h, AT(0, 0), PET_SCALE, st, pet);

    if (st->super) {
        scene_sprite(rect, shield_rows, SPRITE_H(shield_rows), AT(pet->shield_x, pet->shield_y), PET_SCALE, st, pet);
    }
    if (st->police) {
        scene_sprite(rect, police_hat_rows, SPRITE_H(police_hat_rows), AT(pet->hat_x, pet->hat_y), PET_SCALE, st, pet);
    }
    for (uint8_t i = 0; i < st->zzz_step && i < 3; i++) {
        scene_z(rect, x0 + pet->zzz[i][0], y0 + pet->zzz[i][1], pet->zzz[i][2]);
    }
#undef AT

    // Meadow, with a few blades poking up in front of the feet.
    rect(0, ground, width, 12, (hsv_t){85, 255, 110});
    for (int16_t x = 2; x < width - 2; x += 7) {
        int16_t h = (x * 5) % 4 + 2;
        rect(x, ground - h, 2, h, (hsv_t){85, 255, 160});
    }
}
