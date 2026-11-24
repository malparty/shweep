// SPDX-License-Identifier: GPL-2.0-or-later
// Shweep the sheep: pixel art drawn on the right-half TFT.
//
// Each character is one sprite pixel (drawn SHEEP_SCALE x SHEEP_SCALE on screen).
// Edit the art freely: rows must stay SHEEP_W characters wide.
//
//   .  transparent            W  wool        w  wool shadow
//   F  face                   f  face shadow k  ear         i  inner ear
//   o  pupil top  e  eye glint (both face colour when blinking/asleep)
//   p  pupil bottom / closed eyelid
//   r  nose                   n  mouth line
//   m  open mouth (face colour when closed)
//   G  grass over the face (face colour when the mouth is closed)
//   g  grass outside the face (hidden when the mouth is closed)
//   L  leg                    H  hoof

#pragma once

#include "color.h"

#define SHEEP_W 32
#define SHEEP_H 22
#define SHEEP_SCALE 4

// clang-format off
// SHEEP_ART_BEGIN
static const char sheep_art[SHEEP_H][SHEEP_W + 1] = {
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
    "....LL..LL......LL..LL..........",
    "....LL..LL......LL..LL..........",
    "....LL..LL......LL..LL..........",
    "....HH..HH......HH..HH..........",
};
// SHEEP_ART_END
// clang-format on

// Colour of one sprite pixel, given the animation state. Returns false for transparent.
static inline bool sheep_color(char c, bool eyes_open, bool mouth_open, hsv_t *out) {
    switch (c) {
        case 'e':
        case 'o': c = eyes_open ? c : 'F'; break;
        case 'm': c = mouth_open ? 'm' : 'F'; break;
        case 'G': c = mouth_open ? 'g' : 'F'; break;
        case 'g': if (!mouth_open) return false; break;
        default: break;
    }
    switch (c) {
        case 'W': *out = (hsv_t){0, 0, 255}; return true;
        case 'w': *out = (hsv_t){170, 25, 170}; return true;
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
        case 'g': *out = (hsv_t){85, 255, 200}; return true;
        case 'L': *out = (hsv_t){20, 100, 120}; return true;
        case 'H': *out = (hsv_t){0, 0, 75}; return true;
        default: return false;
    }
}
