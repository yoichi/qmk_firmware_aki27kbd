// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>
#include "gr_trackpad65_driver.h"

enum my_keycodes {
  HIGH_SPEED = QK_KB_0,
  LOW_SPEED,
  TGL_V_SCL,
  TGL_H_SCL,
  EN_3_TAP,
  DIS_3_TAP,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ A │ B │ C │ D │ E │ F │ G │ H │ I │ J │ K │ L │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │ F │ G │ H │ I │ J │ K │ L │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │ F │ G │ H │ I │ J │ K │ L │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │ F │ G │ H │ I │ J │ K │ L │
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT(
        KC_ESC , KC_Q,    KC_W,    KC_E,            KC_R,           KC_T, KC_DEL,  KC_F5,   KC_F2,   KC_Y,    KC_U,           KC_I,             KC_O,    KC_P,    KC_MINS,
        KC_TAB , KC_A,    KC_S,    KC_D,            KC_F,           KC_G,                            KC_H,    KC_J,           KC_K,             KC_L,    KC_SCLN, KC_QUOT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,            KC_V,           KC_B,                            KC_N,    KC_M,           KC_COMM,          KC_DOT,  KC_SLSH, KC_BSLS,
                 KC_LGUI, KC_LALT, LSFT_T(KC_LNG2), LT(2, KC_SPC),  MO(1),         LOW_SPEED,      MO(1),   LT(2, KC_ENT),  RSFT_T(KC_LNG1),  KC_LBRC, KC_RBRC
               , KC_BTN3, G(KC_TAB), KC_BTN5, G(KC_D), KC_BTN4, KC_BTN3, G(KC_TAB), G(C(KC_RGHT)), G(KC_D), G(C(KC_LEFT))
    ),

    [1] = LAYOUT(
        DF(3)  , _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______, _______,
        _______, _______, _______, _______, _______, _______,                            KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______,
        _______, _______, _______, _______, _______, _______,                            KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, _______, _______,
                 _______, _______, _______, _______, _______,          _______,          _______, KC_BSPC, _______, _______, _______
               , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [2] = LAYOUT(
        KC_TILD, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _______, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
        KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                            KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_EQL ,
        KC_F11 , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                            KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F12 ,
                 _______, _______, _______, _______, _______,          KC_KB_MUTE,       _______, _______, _______, _______, _______
               , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [3] = LAYOUT(
        _______, _______, _______, EN_3_TAP , _______, _______, TGL_V_SCL, _______, TGL_H_SCL, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, DIS_3_TAP, _______, _______,                                _______, _______, _______, _______, _______, _______ ,
        _______, _______, _______, _______  , _______, _______,                                _______, _______, _______, _______, _______, _______ ,
                 _______, _______, _______  , _______, _______,            DF(0)  ,            _______, _______, _______, _______, _______
               , _______, _______, _______  , _______, _______, _______  , _______, _______  , _______, _______
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    [1] =   { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
    [2] =   { ENCODER_CCW_CW(KC_KB_VOLUME_UP, KC_KB_VOLUME_DOWN) },
    [3] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) }
};

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;
}

typedef enum  {
    SPEED_MODE_LOW,
    SPEED_MODE_NORMAL,
    SPEED_MODE_HIGH
} speed_mode_t;

static speed_mode_t speed_mode = SPEED_MODE_NORMAL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HIGH_SPEED:
      if (record->event.pressed) {
        speed_mode = SPEED_MODE_HIGH;
      } else {
        speed_mode = SPEED_MODE_NORMAL;
      }
      return true;

    case LOW_SPEED:
      if (record->event.pressed) {
        speed_mode = SPEED_MODE_LOW;
      } else {
        speed_mode = SPEED_MODE_NORMAL;
      }
      return true;

    case TGL_V_SCL:
      if (record->event.pressed) {
        trackpad_config.reverse_vertical_scroll = !trackpad_config.reverse_vertical_scroll;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case TGL_H_SCL:
      if (record->event.pressed) {
        trackpad_config.reverse_horizontal_scroll = !trackpad_config.reverse_horizontal_scroll;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case EN_3_TAP:
      if (record->event.pressed) {
        trackpad_config.disable_3fingers_tap = false;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case DIS_3_TAP:
      if (record->event.pressed) {
        trackpad_config.disable_3fingers_tap = true;
        update_trackpad_config(trackpad_config);
      }
      return false;

    default:
      return true;
  }
}

int LOW_SPEED_RATIO = 2;
int HIGH_SPEED_RATIO = 2;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

    if (speed_mode == SPEED_MODE_LOW) {
        mouse_report.x = CONSTRAIN_HID_XY(mouse_report.x / LOW_SPEED_RATIO);
        mouse_report.y = CONSTRAIN_HID_XY(mouse_report.y / LOW_SPEED_RATIO);
    }

    if (speed_mode == SPEED_MODE_HIGH) {
        mouse_report.x = CONSTRAIN_HID_XY(mouse_report.x * HIGH_SPEED_RATIO);
        mouse_report.y = CONSTRAIN_HID_XY(mouse_report.y * HIGH_SPEED_RATIO);
    }

    return mouse_report;
}
