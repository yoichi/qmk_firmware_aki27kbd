/* Copyright 2020 Alexander Tulloh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"
#include <math.h>
#include "cocot36plus.h"
#include "wait.h"
#include "debug.h"
#include <stdio.h>
#include "pointing_device.h"
extern const pointing_device_driver_t pointing_device_driver;


// Invert vertical scroll direction
#ifndef COCOT_SCROLL_INV_DEFAULT
#    define COCOT_SCROLL_INV_DEFAULT 1
#endif

#ifndef COCOT_CPI_OPTIONS
#    define COCOT_CPI_OPTIONS { 200, 400, 800, 1600, 3200 }
#    ifndef COCOT_CPI_DEFAULT
#       define COCOT_CPI_DEFAULT 3
#    endif
#endif
#ifndef COCOT_CPI_DEFAULT
#    define COCOT_CPI_DEFAULT 3
#endif

#ifndef COCOT_SCROLL_DIVIDERS
#    define COCOT_SCROLL_DIVIDERS { 1, 2, 3, 4, 5, 6 }
#    ifndef COCOT_SCROLL_DIV_DEFAULT
#       define COCOT_SCROLL_DIV_DEFAULT 4
#    endif
#endif
#ifndef COCOT_SCROLL_DIV_DEFAULT
#    define COCOT_SCROLL_DIV_DEFAULT 4
#endif


#ifndef COCOT_ROTATION_ANGLE
#    define COCOT_ROTATION_ANGLE { -60, -45, -30, -15, 0, 15, 30, 45, 60 }
#    ifndef COCOT_ROTATION_DEFAULT
#       define COCOT_ROTATION_DEFAULT 2
#    endif
#endif
#ifndef COCOT_ROTATION_DEFAULT
#    define COCOT_ROTATION_DEFAULT 2
#endif


cocot_config_t cocot_config;
uint16_t cpi_array[] = COCOT_CPI_OPTIONS;
uint16_t scrl_div_array[] = COCOT_SCROLL_DIVIDERS;
int16_t angle_array[] = COCOT_ROTATION_ANGLE;
#define CPI_OPTION_SIZE (sizeof(cpi_array) / sizeof(uint16_t))
#define SCRL_DIV_SIZE (sizeof(scrl_div_array) / sizeof(uint16_t))
#define ANGLE_SIZE (sizeof(angle_array) / sizeof(int16_t))


// Scroll Accumulation
static int16_t h_acm       = 0;
static int16_t v_acm       = 0;


void pointing_device_init_kb(void) {
    // set the CPI.
    pointing_device_set_cpi(cpi_array[cocot_config.cpi_idx]);
}


report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {

    double rad = angle_array[cocot_config.rotation_angle] * (M_PI / 180) * -1;
    int8_t x_rev =  + mouse_report.x * cos(rad) - mouse_report.y * sin(rad);
    int8_t y_rev =  + mouse_report.x * sin(rad) + mouse_report.y * cos(rad);

    if (cocot_get_scroll_mode()) {
        // rock scroll direction
        if (abs(x_rev) > abs(y_rev)) {
            y_rev = 0;
        } else {
            x_rev = 0;
        }

        // accumulate scroll
        h_acm += x_rev * cocot_config.scrl_inv;
        v_acm += y_rev * cocot_config.scrl_inv * -1;

        int8_t h_rev = h_acm >> scrl_div_array[cocot_config.scrl_div];
        int8_t v_rev = v_acm >> scrl_div_array[cocot_config.scrl_div];

        // clear accumulated scroll on assignment

        if (h_rev != 0) {
            if (mouse_report.h + h_rev > 127) {
                h_rev = 127 - mouse_report.h;
            } else if (mouse_report.h + h_rev < -127) {
                h_rev = -127 - mouse_report.h;
            }
            mouse_report.h += h_rev;
            h_acm -= h_rev << scrl_div_array[cocot_config.scrl_div];
        }
        if (v_rev != 0) {
            if (mouse_report.v + v_rev > 127) {
                v_rev = 127 - mouse_report.v;
            } else if (mouse_report.v + v_rev < -127) {
                v_rev = -127 - mouse_report.v;
            }
            mouse_report.v += v_rev;
            v_acm -= v_rev << scrl_div_array[cocot_config.scrl_div];
        }

        mouse_report.x = 0;
        mouse_report.y = 0;
    } else {
        mouse_report.x = -x_rev;
        mouse_report.y = y_rev;
    }

    return pointing_device_task_user(mouse_report);
}



bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    // xprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
    
    if (!process_record_user(keycode, record)) return false;

    switch (keycode) {
#ifndef MOUSEKEY_ENABLE
        // process KC_MS_BTN1~8 by myself
        // See process_action() in quantum/action.c for details.
        case KC_MS_BTN1 ... KC_MS_BTN8: {
            extern void register_button(bool, enum mouse_buttons);
            register_button(record->event.pressed, MOUSE_BTN_MASK(keycode - KC_MS_BTN1));
            return false;
        }
#endif

    }

    if (keycode == CPI_SW && record->event.pressed) {
        cocot_config.cpi_idx = (cocot_config.cpi_idx + 1) % CPI_OPTION_SIZE;
        eeconfig_update_kb(cocot_config.raw);
        pointing_device_set_cpi(cpi_array[cocot_config.cpi_idx]);
    }

    if (keycode == SCRL_SW && record->event.pressed) {
        cocot_config.scrl_div = (cocot_config.scrl_div + 1) % SCRL_DIV_SIZE;
        eeconfig_update_kb(cocot_config.raw);
    }
    
    if (keycode == ROT_R15 && record->event.pressed) {
        cocot_config.rotation_angle = (cocot_config.rotation_angle + 1) % ANGLE_SIZE;
        eeconfig_update_kb(cocot_config.raw);
    }

    if (keycode == ROT_L15 && record->event.pressed) {
        cocot_config.rotation_angle = (ANGLE_SIZE + cocot_config.rotation_angle - 1) % ANGLE_SIZE;
        eeconfig_update_kb(cocot_config.raw);
    }

    if (keycode == SCRL_IN && record->event.pressed) {
        cocot_config.scrl_inv = - cocot_config.scrl_inv;
        eeconfig_update_kb(cocot_config.raw);
    }

    if (keycode == SCRL_TO && record->event.pressed) {
        { cocot_config.scrl_mode ^= 1; }
    }

    if (keycode == SCRL_MO) {
        { cocot_config.scrl_mode ^= 1; }
    }

    return true;
}


void eeconfig_init_kb(void) {
    cocot_config.cpi_idx = COCOT_CPI_DEFAULT;
    cocot_config.scrl_div = COCOT_SCROLL_DIV_DEFAULT;
    cocot_config.rotation_angle = COCOT_ROTATION_DEFAULT;
    cocot_config.scrl_inv = COCOT_SCROLL_INV_DEFAULT;
    cocot_config.scrl_mode = false;
    eeconfig_update_kb(cocot_config.raw);
    eeconfig_init_user();
}


void matrix_init_kb(void) {
    // is safe to just read CPI setting since matrix init
    // comes before pointing device init.
    cocot_config.raw = eeconfig_read_kb();
    if (cocot_config.cpi_idx > CPI_OPTION_SIZE) // || cocot_config.scrl_div > SCRL_DIV_SIZE || cocot_config.rotation_angle > ANGLE_SIZE)
    {
        eeconfig_init_kb();
    }
    matrix_init_user();
}


bool cocot_get_scroll_mode(void) {
    return cocot_config.scrl_mode;
}

void cocot_set_scroll_mode(bool mode) {
    cocot_config.scrl_mode = mode;
}













#ifdef RGB_MATRIX_ENABLE
    led_config_t g_led_config = { {
    // Key Matrix to LED Index
        {  9,10,11,12,13,40,41,42,43,44 },
        { 14,15,16,17,18,35,36,37,38,39 },
        { 19,20,21,22,23,30,31,32,33,34 },
        { NO_LED,24,25,26,NO_LED,NO_LED,27,28,29,NO_LED}
    }, {
    // LED Index to Physical Position
        // Underglow
        { 132,  10 }, { 180,  10 }, { 180,  50 }, { 132,  50 }, { 108,  50 }, { 84,  50 }, { 12,  50 }, { 12,  10 }, { 84,  10 },
        // Per Key Backlight
        // Left Side
        {   0,   0 }, {  24,   0 }, {  48,   0 }, {  72,   0 }, {  96,   0 },
        {   0,  20 }, {  24,  20 }, {  48,  20 }, {  72,  20 }, {  96,  20 },
        {   0,  40 }, {  24,  40 }, {  48,  40 }, {  72,  40 }, {  96,  40 },
                                    {  48,  60 }, {  72,  60 }, {  96,  60 },
        // Right Side
        { 120,  60 }, { 144,  60 }, { 168,  60 },
        { 120,  40 }, { 144,  40 }, { 168,  40 }, { 192,  40 }, { 216,  40 },
        { 120,  20 }, { 144,  20 }, { 168,  20 }, { 192,  20 }, { 216,  20 },
        { 120,   0 }, { 144,   0 }, { 168,   0 }, { 192,   0 }, { 216,   0 }
    }, {
    // LED Index to Flag
        2, 2, 2, 2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4
} };
#endif


// OLED utility
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}


static const char PROGMEM cocot_logo[] = {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4,
    0};

void render_logo(void) {
    oled_write_P(cocot_logo, false);
};



void oled_write_layer_state(void) {

    oled_write_P(PSTR("Layer"), false);
    // int cpi = pointing_device_get_cpi();
    int cpi = cpi_array[cocot_config.cpi_idx];
    int scroll_div = scrl_div_array[cocot_config.scrl_div];
    int angle_origin = angle_array[cocot_config.rotation_angle];
    int angle;
    if (angle_origin > 360) {
        angle = angle_origin - 65536;
    } else {
        angle = angle_origin;
    }

    //int cpi = 1000;
    //int scroll_div = 7;
    //int angle = -90;

    char buf1[6];
    char buf2[6];
    char buf3[8];
    snprintf(buf1, 6, "%4d", cpi);
    snprintf(buf2, 6, "%1d", scroll_div);
    snprintf(buf3, 8, "%2d", abs(angle));
    
    oled_write_P    (get_u8_str(get_highest_layer(layer_state), ' '), false);
    
    oled_write_P(PSTR("/"), false);
    
    if (cocot_get_scroll_mode()){
        oled_write_P(PSTR("S"), false);
    } else{
        oled_write_P(PSTR("C"), false);
    }
    
    /*
    if (state == SCROLLING) {
        oled_write_P(PSTR("S"), false);
    } else {
        oled_write_P(PSTR("C"), false);
    }
    */
    
    oled_write_P(PSTR("/"), false);
    oled_write(buf1, false);
    oled_write_P(PSTR("/"), false);
    oled_write(buf2, false);
    oled_write_P(PSTR("/"), false);
    if (angle < 0) {
        oled_write_P(PSTR("-"), false);
    } else {
        oled_write_P(PSTR(" "), false);
    }
    oled_write(buf3, false);
}


bool oled_task_user(void) {
    render_logo();
    oled_write_layer_state();
    return false;
}


#endif
