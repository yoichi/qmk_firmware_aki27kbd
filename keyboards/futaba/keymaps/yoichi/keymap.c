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
  EN_TAP,
  DIS_TAP,
  ZOOM_IN,
  ZOOM_OUT,
  IME_TGL,
  PAD_3_RGHT,
  PAD_3_LEFT,
  PAD_3_DOWN,
  PAD_3_UP,
  PAD_4_RGHT,
  PAD_4_LEFT,
  PAD_4_DOWN,
  PAD_4_UP,
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
   LT(2,KC_TAB), KC_Q,    KC_W,    KC_E,   KC_R,    KC_T,     MS_BTN1, MS_BTN3, MS_BTN2,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, LT(1,KC_EQL),
 LCTL_T(KC_ESC), KC_A,    KC_S,    KC_D,   KC_F,    KC_G,                                   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, RCTL_T(KC_MINS),
        KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,    KC_B,                                   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                 KC_LALT, KC_LGUI, MS_BTN1, LT(1, KC_SPC),  MO(3),         LOW_SPEED,    KC_BSPC,   LT(2, KC_ENT),  MS_BTN2,  KC_RALT, KC_RGUI
               , MS_BTN3, PAD_3_UP, PAD_3_RGHT, PAD_3_DOWN, PAD_3_LEFT, MS_BTN3, PAD_4_UP, PAD_4_RGHT, PAD_4_DOWN, PAD_4_LEFT
    ),

    [1] = LAYOUT(
  LT(3,KC_QUOT),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, _______, _______, _______,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSLS,
        _______, KC_INT1, KC_INT3,  KC_GRV, KC_LBRC, KC_RBRC,                            KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT,  KC_INS, _______,
        _______, KC_CAPS,_______,S(KC_HOME),S(KC_END),KC_DEL,                            _______, _______, KC_COMM,  KC_DOT, KC_SLSH, _______,
                 _______, _______, KC_HOME, _______,  KC_END,          _______,          _______, _______, _______, _______, _______
               , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [2] = LAYOUT(
     S(KC_QUOT), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), _______, _______, _______, S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0),S(KC_BSLS),
_______,S(KC_INT1),S(KC_INT3),S(KC_GRV),S(KC_LBRC),S(KC_RBRC),                    S(KC_LEFT),S(KC_DOWN),S(KC_UP),S(KC_RGHT), _______, _______,
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
              _______, _______, _______,S(KC_HOME),S(KC_END),          KC_KB_MUTE,       _______, _______, _______, _______, _______
               , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [3] = LAYOUT(
          DF(4), G(KC_1), G(KC_2), G(KC_3), G(KC_4), G(KC_5), _______, _______, _______, G(KC_6), G(KC_7), G(KC_8), G(KC_9), KC_BRIU, KC_VOLU,
          _______, _______, _______, _______, _______, KC_PGUP,                         C(KC_LEFT),C(KC_DOWN),C(KC_UP),C(KC_RGHT), KC_BRID, KC_VOLD,
   _______,RWIN(KC_1),RWIN(KC_2),RWIN(KC_3),RWIN(KC_4),KC_PGDN,             RWIN(KC_LEFT),RWIN(KC_DOWN),RWIN(KC_UP),RWIN(KC_RGHT), _______, KC_MUTE,
                 _______, _______, _______  , _______, _______,        _______,          IME_TGL, _______, _______, _______, _______
               , _______, _______, _______  , _______, _______, _______  , _______, _______  , _______, _______
    ),
    [4] = LAYOUT(
        _______, _______,  EN_TAP, EN_3_TAP , _______, _______, TGL_V_SCL, _______, TGL_H_SCL, _______, _______, _______, _______, _______, _______,
        QK_BOOT, _______, DIS_TAP, DIS_3_TAP, _______, _______,                                _______, _______, _______, _______, _______,  EE_CLR,
        _______, _______, _______, _______  , _______, _______,                                _______, _______, AG_LSWP, AG_LNRM, _______, _______,
                 _______, _______, _______  , _______, _______,            DF(0)  ,            _______, _______, _______, _______, _______
               , _______, _______, _______  , _______, _______, _______  , _______, _______  , _______, _______
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    [1] =   { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
    [2] =   { ENCODER_CCW_CW(KC_KB_VOLUME_UP, KC_KB_VOLUME_DOWN) },
    [3] =   { ENCODER_CCW_CW(ZOOM_OUT, ZOOM_IN) },
    [4] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) }
};

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;

    trackpad_config.disable_tap = true;
    update_trackpad_config(trackpad_config);
}

#if defined(OS_DETECTION_ENABLE)
bool process_detected_host_os_user(os_variant_t detected_os) {
#if defined(MAGIC_ENABLE)
    eeconfig_read_keymap(&keymap_config);
#endif
    if (detected_os == OS_WINDOWS) {
        trackpad_config.reverse_vertical_scroll = false;
        trackpad_config.reverse_horizontal_scroll = true;
        trackpad_config.scroll_scale_percent = 100;
        update_trackpad_config(trackpad_config);
#if defined(MAGIC_ENABLE)
        keymap_config.swap_lalt_lgui = true;
        keymap_config.swap_ralt_rgui = false;
#endif
    } else {
        trackpad_config.reverse_vertical_scroll = true;
        trackpad_config.reverse_horizontal_scroll = false;
        trackpad_config.scroll_scale_percent = 5;
        update_trackpad_config(trackpad_config);
#if defined(MAGIC_ENABLE)
        keymap_config.swap_lalt_lgui = false;
        keymap_config.swap_ralt_rgui = false;
#endif
    }
#if defined(MAGIC_ENABLE)
    eeconfig_update_keymap(&keymap_config);
#endif
    return 0;
}
#endif

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

    case EN_TAP:
      if (record->event.pressed) {
        trackpad_config.disable_tap = false;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case DIS_TAP:
      if (record->event.pressed) {
        trackpad_config.disable_tap = true;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case ZOOM_IN:
      {
        uint16_t code = G(KC_EQL);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = C(KC_EQL);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case ZOOM_OUT:
      {
        uint16_t code = G(KC_MINS);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = C(KC_MINS);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;

    case IME_TGL:
      {
        uint16_t mod = KC_LGUI;
        uint16_t code = KC_SPC;
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          mod = KC_RALT;
          code = KC_GRV;
        }
#endif
        if (record->event.pressed) {
          register_code16(mod);
          wait_ms(10);
          tap_code16(code);
          unregister_code16(mod);
        }
      }
      return false;

    case PAD_3_RGHT:
      {
        uint16_t code = C(KC_LEFT);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = MS_BTN4;
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case PAD_3_LEFT:
      {
        uint16_t code = C(KC_RGHT);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = MS_BTN5;
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case PAD_3_DOWN:
      {
        uint16_t code = C(KC_DOWN);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = G(KC_D);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case PAD_3_UP:
      {
        uint16_t code = C(KC_UP);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = G(KC_TAB);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;

    case PAD_4_RGHT:
      {
        uint16_t code = LCA(KC_RGHT);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = RGUI(KC_RGHT);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case PAD_4_LEFT:
      {
        uint16_t code = LCA(KC_LEFT);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = RGUI(KC_LEFT);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case PAD_4_DOWN:
      {
        uint16_t code = LCA(KC_DOWN);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = RGUI(KC_DOWN);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
      }
      return false;
    case PAD_4_UP:
      {
        uint16_t code = LCA(KC_UP);
#ifdef OS_DETECTION_ENABLE
        if (detected_host_os() == OS_WINDOWS) {
          code = RGUI(KC_UP);
        }
#endif
        if (record->event.pressed) {
          register_code16(code);
        } else {
          unregister_code16(code);
        }
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
