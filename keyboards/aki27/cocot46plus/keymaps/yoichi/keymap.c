/*
Copyright 2022 aki27

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "quantum.h"
#include "os_detection.h"


// Defines names for use in layer keycodes and the keymap
enum layer_number {
    _BASE = 0,
    _NUMBER = 1,
    _SYMBOL = 2,
    _MEDIA = 3,
    _CONFIG = 4,
};


enum custom_user_keycodes {
    IME_TGL = QK_USER_0,
    VD_LEFT = QK_USER_1,
    VD_RGHT = QK_USER_2,
    LOCK_PC = QK_USER_3,

    // Reuse unused basic keycodes
    MY_PIPE = KC_NUBS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
LT(_SYMBOL,KC_TAB), KC_Q,  KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, LT(_NUMBER,KC_EQL),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
LCTL_T(KC_ESC),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, RCTL_T(KC_MINS),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
             KC_LALT, KC_LGUI, LT(_NUMBER,KC_SPC), KC_BTN1,      KC_BTN2,                IME_TGL, KC_BSPC, LT(_SYMBOL,KC_ENT), KC_RGUI, RALT_T(KC_ESC),
                                                                 XXXXXXX,    LOCK_PC,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_NUMBER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
LT(_MEDIA,KC_QUOT), KC_1,  KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSLS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, KC_INT1, KC_INT3,  KC_GRV, KC_LBRC, KC_RBRC,                                       KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,  KC_INS, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LSFT, KC_CAPS,_______,S(KC_HOME),S(KC_END),KC_DEL,                                       KC_PSCR, _______, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, KC_HOME, _______,  KC_END,      KC_LNG2,                KC_LNG1, IME_TGL, TT(_MEDIA), _______, RCTL(KC_RALT),
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_SYMBOL] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
   S(KC_QUOT), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),                                       S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0),LT(_MEDIA,MY_PIPE),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,S(KC_INT1),S(KC_INT3),S(KC_GRV),S(KC_LBRC),S(KC_RBRC),                       S(KC_LEFT),S(KC_DOWN),S(KC_UP),S(KC_RGHT), _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                         KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
               LCTL(KC_LGUI), KC_LGUI, TT(_MEDIA), _______,      _______,                _______, _______, _______, _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_MEDIA] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
  MO(_CONFIG), G(KC_1), G(KC_2), G(KC_3), G(KC_4), G(KC_5),                                       G(KC_6), G(KC_7), G(KC_8), G(KC_9), KC_BRIU, KC_VOLU,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      QK_RBT , _______, _______, _______, _______, KC_PGUP,                                C(KC_LEFT),C(KC_DOWN),C(KC_UP),C(KC_RGHT), KC_BRID, KC_VOLD,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LSFT, _______, _______, _______, _______, KC_PGDN,                        LCA(KC_LEFT),LCA(KC_DOWN),LCA(KC_UP),LCA(KC_RGHT), _______, KC_MUTE,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,      _______,                _______, _______, _______, _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_CONFIG] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,                                       SCRL_TO,  CPI_SW, SCRL_SW, ROT_L15, ROT_R15, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      QK_BOOT, XXXXXXX, UG_VALU, UG_SATU, UG_HUEU, UG_NEXT,                                       SCRL_MO, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, UG_VALD, UG_SATD, UG_HUED, UG_PREV,                                       SCRL_IN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______, QK_KEY_OVERRIDE_ON, QK_KEY_OVERRIDE_OFF, _______, _______, _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(VD_RGHT, VD_LEFT) },
#ifdef MOUSEKEY_ENABLE
    [_NUMBER] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_SYMBOL] = { ENCODER_CCW_CW(MS_WHLR, MS_WHLL) },
#else
    [_NUMBER] = { ENCODER_CCW_CW(KC_DOWN, KC_UP) },
    [_SYMBOL] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
#endif
    [_MEDIA] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_CONFIG] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
};
#endif



layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _NUMBER:
#ifdef RGBLIGHT_ENABLE
        rgblight_sethsv_range(HSV_BLUE, 0, 2);
#endif
        cocot_set_scroll_mode(true);
        break;
    case _SYMBOL:
#ifdef RGBLIGHT_ENABLE
        rgblight_sethsv_range(HSV_RED, 0, 2);
#endif
        cocot_set_scroll_mode(true);
        break;
    case _MEDIA:
#ifdef RGBLIGHT_ENABLE
        rgblight_sethsv_range(HSV_GREEN, 0, 2);
#endif
        cocot_set_scroll_mode(false);
        break;
    case _CONFIG:
#ifdef RGBLIGHT_ENABLE
        rgblight_sethsv_range(HSV_YELLOW, 0, 2);
#endif
        cocot_set_scroll_mode(false);
        break;
    default:
#ifdef RGBLIGHT_ENABLE
        rgblight_sethsv_range( 0, 0, 0, 0, 2);
#endif
        cocot_set_scroll_mode(false);
        break;
    }
#ifdef RGBLIGHT_ENABLE
    rgblight_set_effect_range( 2, 10);
#endif
      return state;
};


#ifdef OLED_ENABLE
bool oled_task_user(void) {
    render_logo();
    oled_write_layer_state();
    return false;
}
#endif

#ifdef KEY_OVERRIDE_ENABLE
#include "process_key_override.h"
#include "keymap_japanese.h"
const key_override_t *key_overrides[] = {
    &ko_make_basic(MOD_MASK_SHIFT, KC_2, JP_AT), // @
    &ko_make_basic(MOD_MASK_SHIFT, S(KC_2), JP_AT), // @
    &ko_make_basic(0, S(KC_2), JP_AT), // @

    &ko_make_basic(MOD_MASK_SHIFT, KC_6, JP_CIRC), // ^
    &ko_make_basic(MOD_MASK_SHIFT, S(KC_6), JP_CIRC), // ^
    &ko_make_basic(0, S(KC_6), JP_CIRC), // ^

    &ko_make_basic(MOD_MASK_SHIFT, KC_7, JP_AMPR), // &
    &ko_make_basic(0, S(KC_7), JP_AMPR), // &

    &ko_make_basic(MOD_MASK_SHIFT, KC_8, JP_ASTR), // *
    &ko_make_basic(0, S(KC_8), JP_ASTR), // *

    &ko_make_basic(MOD_MASK_SHIFT, KC_9, JP_LPRN), // (
    &ko_make_basic(0, S(KC_9), JP_LPRN), // (

    &ko_make_basic(MOD_MASK_SHIFT, KC_0, JP_RPRN), // )
    &ko_make_basic(0, S(KC_0), JP_RPRN), // )

    &ko_make_basic(MOD_MASK_SHIFT, KC_SCLN, JP_COLN), // :

    // LT(_NUMBER,KC_EQL) is handled in process_record_user

    // we can apply overrides but loose RCTL_T() effect
    &ko_make_basic(MOD_MASK_SHIFT, RCTL_T(KC_MINS), JP_UNDS), // _

    // we can apply overrides but loose LT(layer,) effect
    &ko_make_with_layers_and_negmods(0, LT(_MEDIA,KC_QUOT), JP_QUOT, ~0, (uint8_t) MOD_MASK_SHIFT), // '
    &ko_make_basic(MOD_MASK_SHIFT, LT(_MEDIA,KC_QUOT), JP_DQUO), // "
    &ko_make_basic(0, S(KC_QUOT), JP_DQUO), // "

    &ko_make_with_layers_and_negmods(0, KC_GRV, JP_GRV, ~0, (uint8_t) MOD_MASK_SHIFT), // `
    &ko_make_basic(MOD_MASK_SHIFT, KC_GRV, JP_TILD), // ~
    &ko_make_basic(0, S(KC_GRV), JP_TILD), // ~

    &ko_make_with_layers_and_negmods(0, KC_BSLS, JP_BSLS, ~0, (uint8_t) MOD_MASK_SHIFT), // (backslash)
    &ko_make_basic(MOD_MASK_SHIFT, KC_BSLS, JP_PIPE), // |
    // LT(_MEDIA,MY_PIPE) is handled in process_record_user

    &ko_make_with_layers_and_negmods(0, KC_LBRC, JP_LBRC, ~0, (uint8_t) MOD_MASK_SHIFT), // [
    &ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, JP_LCBR), // {
    &ko_make_basic(0, S(KC_LBRC), JP_LCBR), // {

    &ko_make_with_layers_and_negmods(0, KC_RBRC, JP_RBRC, ~0, (uint8_t) MOD_MASK_SHIFT), // ]
    &ko_make_basic(MOD_MASK_SHIFT, KC_RBRC, JP_RCBR), // }
    &ko_make_basic(0, S(KC_RBRC), JP_RCBR), // }

    &ko_make_basic(0, KC_CAPS, JP_CAPS), // (CapsLock)
};
#endif

#if defined(OS_DETECTION_ENABLE)
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_WINDOWS:
            cocot_config.scrl_inv = 1;
            eeconfig_update_kb(cocot_config.raw);
#ifdef KEY_OVERRIDE_ENABLE
            key_override_on();
#endif
            break;
        case OS_MACOS:
            cocot_config.scrl_inv = -1;
            eeconfig_update_kb(cocot_config.raw);
#ifdef KEY_OVERRIDE_ENABLE
            key_override_off();
#endif
            break;
        default:
            break;
    }
    return 0;
}
#endif

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef KEY_OVERRIDE_ENABLE
        case LT(_NUMBER,KC_EQL):
            if (key_override_is_enabled()) {
                if (record->tap.count) {
                    static uint16_t kc;
                    if (record->event.pressed) {
                        uint8_t mod_state = get_mods();
                        if (mod_state & MOD_MASK_SHIFT) {
                            del_mods(MOD_MASK_SHIFT);
                            kc = JP_PLUS;
                        } else if (mod_state & MOD_MASK_CTRL) {
                            // treat Ctrl+= as Zoom-in like US layout (=, + are on the same key)
                            // cf. Ctrl+= is treated as Zoom-out in JP layout (-, = are on the same key)
                            del_mods(MOD_MASK_SHIFT);
                            kc = JP_SCLN; // ;, + are on the same key in JIS layout
                        } else {
                            kc = JP_EQL;
                        }
                        register_code16(kc);
                        set_mods(mod_state);
                        return false;
                    } else if (kc) {
                        unregister_code16(kc);
                        kc = 0;
                        return false;
                    }
                }
            }
            break;
#endif
        case LT(_MEDIA,MY_PIPE):
            if (record->tap.count) {
                static uint16_t kc;
                if (record->event.pressed) {
#ifdef KEY_OVERRIDE_ENABLE
                    if (key_override_is_enabled()) {
                        kc = JP_PIPE;
                    } else {
#else
                    {
#endif
                        kc = S(KC_BSLS);
                    }
                    register_code16(kc);
                    return false;
                } else if (kc) {
                    unregister_code16(kc);
                    kc = 0;
                    return false;
                }
            }
            break;
#if defined(OS_DETECTION_ENABLE)
        case KC_LALT:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(KC_LGUI);
                } else {
                    unregister_code16(KC_LGUI);
                }
                return false;
            default:
                break;
            }
            break;
        case KC_LGUI:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(KC_LALT);
                } else {
                    unregister_code16(KC_LALT);
                }
                return false;
            default:
                break;
            }
            break;
        case LCA(KC_LEFT):
        case LCA(KC_DOWN):
        case LCA(KC_UP):
        case LCA(KC_RGHT):
            switch (detected_host_os()) {
            case OS_WINDOWS:
                keycode ^= (QK_LCTL | QK_LALT);
                keycode = RWIN(keycode);
                if (record->event.pressed) {
                    register_code16(keycode);
                } else {
                    unregister_code16(keycode);
                }
                return false;
            default:
                break;
            }
            break;
        case C(KC_LEFT):
        case C(KC_DOWN):
        case C(KC_UP):
        case C(KC_RGHT):
            switch (detected_host_os()) {
            case OS_WINDOWS:
                keycode = RWIN(keycode);
                if (record->event.pressed) {
                    register_code16(keycode);
                } else {
                    unregister_code16(keycode);
                }
                return false;
            default:
                break;
            }
            break;
        case IME_TGL:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
#ifdef KEY_OVERRIDE_ENABLE
                    if (key_override_is_enabled()) {
                        tap_code16(JP_ZKHK);
                    } else {
#else
                    {
#endif
                        register_code16(KC_RALT);
                        wait_ms(10);
                        tap_code16(KC_GRV);
                        unregister_code16(KC_RALT);
                    }
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(KC_LGUI);
                    wait_ms(10);
                    tap_code16(KC_SPC);
                    unregister_code16(KC_LGUI);
                }
                return false;
            }
            break;
        case LOCK_PC:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(G(KC_L));
                } else {
                    unregister_code16(G(KC_L));
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(G(C(KC_Q)));
                } else {
                    unregister_code16(G(C(KC_Q)));
                }
                return false;
            }
            break;
        case VD_LEFT:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(RGUI(C(KC_LEFT)));
                } else {
                    unregister_code16(RGUI(C(KC_LEFT)));
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(C(KC_LEFT));
                } else {
                    unregister_code16(C(KC_LEFT));
                }
                return false;
            }
            break;
        case VD_RGHT:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(RGUI(C(KC_RGHT)));
                } else {
                    unregister_code16(RGUI(C(KC_RGHT)));
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(C(KC_RGHT));
                } else {
                    unregister_code16(C(KC_RGHT));
                }
                return false;
            }
            break;
#endif
#ifdef MOUSEKEY_ENABLE
        case MS_WHLU:
        case MS_WHLD:
            if (cocot_config.scrl_inv < 0) {
                extern void register_mouse(uint8_t mouse_keycode, bool pressed);
                register_mouse(keycode == MS_WHLU ? MS_WHLD : MS_WHLU, record->event.pressed);
                return false;
            }
            break;
        case MS_WHLL:
        case MS_WHLR:
            if (cocot_config.scrl_inv < 0) {
                extern void register_mouse(uint8_t mouse_keycode, bool pressed);
                register_mouse(keycode == MS_WHLL ? MS_WHLR : MS_WHLL, record->event.pressed);
                return false;
            }
            break;
#endif
        default:
            break;
    }
    return true;
}

#ifndef ENCODER_MAP_ENABLE
#ifdef VIA_DUMMY_KEY_FOR_ENCODER
keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 2},
    .pressed = false,
    .type = KEY_EVENT
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 5},
    .pressed = false,
    .type = KEY_EVENT
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index != 0) {
        return true;
    }

    if (clockwise) {
        encoder1_cw.pressed = true;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    } else {
        encoder1_ccw.pressed = true;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }
    return false;
}

void matrix_scan_user(void) {
    if (encoder1_ccw.pressed) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (encoder1_cw.pressed) {
        encoder1_cw.pressed = false;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    }
}
#else // !VIA_DUMMY_KEY_FOR_ENCODER
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index != 0) {
        return true;
    }

    layer_state_t layer = get_highest_layer(layer_state | default_layer_state);
    uint16_t keycode;
    if (clockwise) {
        switch (layer) {
            case _NUMBER:
#ifdef MOUSEKEY_ENABLE
                keycode = MS_WHLD;
#else
                keycode = KC_UP;
#endif
                break;
            case _SYMBOL:
#ifdef MOUSEKEY_ENABLE
                keycode = MS_WHLL;
#else
                keycode = KC_RGHT;
#endif
                break;
            case _MEDIA:
#if defined(OS_DETECTION_ENABLE)
                if (detected_host_os() == OS_WINDOWS) {
                    keycode = C(KC_EQL);
                    break;
                }
#endif
                keycode = G(KC_EQL);
                break;
            case _CONFIG:
                // RGB_HUI cannot be handled by tap_code16_delay
                rgblight_increase_hue();
                return false;
            default:
                return true; // need process in encoder_update_kb
        }
    } else { // counter clockwise
        switch (layer) {
            case _NUMBER:
#ifdef MOUSEKEY_ENABLE
                keycode = MS_WHLU;
#else
                keycode = KC_DOWN;
#endif
                break;
            case _SYMBOL:
#ifdef MOUSEKEY_ENABLE
                keycode = MS_WHLR;
#else
                keycode = KC_LEFT;
#endif
                break;
            case _MEDIA:
#if defined(OS_DETECTION_ENABLE)
                if (detected_host_os() == OS_WINDOWS) {
                    keycode = C(KC_MINS);
                    break;
                }
#endif
                keycode = G(KC_MINS);
                break;
            case _CONFIG:
                // RGB_HUD cannot be handled by tap_code16_delay
                rgblight_decrease_hue();
                return false;
            default:
                return true; // need process in encoder_update_kb
         }
    }
    tap_code16_delay(keycode, 10);
    return false;
}
#endif
#endif // ENCODER_MAP_ENABLE
