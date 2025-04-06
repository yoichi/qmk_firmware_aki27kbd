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
    _LOWER = 1,
    _RAISE = 2,
    _TRACKBALL = 3,
    _Layer4 = 4,
    _Layer5 = 5,
    _Layer6 = 6
};


enum custom_user_keycodes {
    IME_TGL = QK_USER_0,
    ZOOM_IN = QK_USER_1,
    ZOOM_OUT = QK_USER_2,
};


#ifdef TAP_DANCE_ENABLE
// Tap Dance
enum {
    TD_S_IME,
};

void dance_s_ime_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(KC_RSFT);
    } else if (detected_host_os() == OS_WINDOWS) {
        register_code16(KC_RALT);
        wait_ms(10);
        tap_code16(KC_GRV);
        unregister_code16(KC_RALT);
    } else {
        register_code16(G(KC_SPC));
    }
}

void dance_s_ime_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code16(KC_RSFT);
    } else if (detected_host_os() == OS_WINDOWS) {
    } else {
        unregister_code16(G(KC_SPC));
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_S_IME] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_s_ime_finished, dance_s_ime_reset),
};
#endif // TAP_DANCE_ENABLE

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
 LT(2,KC_TAB),    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, LT(1,KC_EQL),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
LCTL_T(KC_ESC),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, RCTL_T(KC_MINS),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
#ifdef TAP_DANCE_ENABLE
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, TD(TD_S_IME),
#else
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
#endif
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                   KC_LALT, KC_LGUI, LT(1,KC_SPC), KC_BTN1,      KC_BTN2,                IME_TGL, KC_BSPC, LT(2,KC_ENT), KC_RGUI, RALT_T(KC_ESC),
                                                                 XXXXXXX, KC_MS_BTN3,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_LOWER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
        TT(3),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, KC_INT1, KC_INT3,  KC_GRV, KC_LBRC, KC_RBRC,                                       KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,  KC_INS, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______,_______,S(KC_HOME),S(KC_END),KC_DEL,                                       KC_QUOT, KC_BSLS, _______, _______, KC_CAPS, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, KC_HOME, _______,  KC_END,      KC_LNG2,                KC_LNG1, _______,   TT(3), _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_RAISE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),                                       S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0),   TT(3),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,S(KC_INT1),S(KC_INT3),S(KC_GRV),S(KC_LBRC),S(KC_RBRC),                                KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                  S(KC_QUOT),S(KC_BSLS), _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, KC_BTN2,   TT(3), _______,      _______,                _______, _______, _______, _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_TRACKBALL] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
        MO(4), G(KC_1), G(KC_2), G(KC_3), G(KC_4), G(KC_5),                                       G(KC_6), G(KC_7), G(KC_8), G(KC_9), KC_BRMU, KC_VOLU,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,S(KC_LEFT),S(KC_DOWN),S(KC_UP),S(KC_RGHT),KC_PGUP,                           C(KC_LEFT),C(KC_DOWN),C(KC_UP),C(KC_RGHT), KC_BRMD, KC_VOLD,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,RWIN(KC_1),RWIN(KC_2),RWIN(KC_3),RWIN(KC_4),KC_PGDN,             RWIN(KC_LEFT),RWIN(KC_DOWN),RWIN(KC_UP),RWIN(KC_RGHT), _______, KC_MUTE,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,      _______,                _______, _______, _______, _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_Layer4] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,                                       SCRL_TO,  CPI_SW, SCRL_SW, ROT_L15, ROT_R15, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      QK_BOOT, XXXXXXX, UG_VALU, UG_SATU, UG_HUEU, UG_NEXT,                                       SCRL_MO, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, UG_VALD, UG_SATD, UG_HUED, UG_PREV,                                       SCRL_IN, XXXXXXX, AG_LSWP, AG_LNRM, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,      _______,                _______, _______, _______, _______, _______,
                                                                 XXXXXXX,    _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_Layer5] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,             XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
                                                                 XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_Layer6] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,             XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
                                                                 XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [2] = { ENCODER_CCW_CW(MS_WHLR, MS_WHLL) },
    [3] = { ENCODER_CCW_CW(ZOOM_OUT, ZOOM_IN) },
    [4] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [5] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [6] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
};
#endif



layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _LOWER:
        rgblight_sethsv_range(HSV_BLUE, 0, 2);
        cocot_set_scroll_mode(true);
        break;
    case _RAISE:
        rgblight_sethsv_range(HSV_RED, 0, 2);
        cocot_set_scroll_mode(true);
        break;
    case _TRACKBALL:
        rgblight_sethsv_range(HSV_GREEN, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    case _Layer4:
        rgblight_sethsv_range(HSV_YELLOW, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    case _Layer5:
        rgblight_sethsv_range(HSV_CYAN, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    case _Layer6:
        rgblight_sethsv_range(HSV_ORANGE, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    default:
        rgblight_sethsv_range( 0, 0, 0, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    }
    rgblight_set_effect_range( 2, 10);
      return state;
};


#ifdef OLED_ENABLE
bool oled_task_user(void) {
    render_logo();
    oled_write_layer_state();
    return false;
}
#endif

#if defined(OS_DETECTION_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
uint32_t os_detect_callback(uint32_t trigger_time, void *cb_arg) {
#if defined(MAGIC_ENABLE)
    keymap_config.raw = eeconfig_read_keymap();
#endif
    switch (detected_host_os()) {
        case OS_WINDOWS:
            cocot_config.scrl_inv = 1;
            eeconfig_update_kb(cocot_config.raw);
#if defined(MAGIC_ENABLE)
            keymap_config.swap_lalt_lgui = true;
            keymap_config.swap_ralt_rgui = false;
#endif
            break;
        case OS_MACOS:
            cocot_config.scrl_inv = -1;
            eeconfig_update_kb(cocot_config.raw);
#if defined(MAGIC_ENABLE)
            keymap_config.swap_lalt_lgui = false;
            keymap_config.swap_ralt_rgui = false;
#endif
            break;
        default:
            break;
    }
#if defined(MAGIC_ENABLE)
    eeconfig_update_keymap(keymap_config.raw);
#endif
    return 0;
}
#endif

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif
#if defined(OS_DETECTION_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
    defer_exec(400, os_detect_callback, NULL);
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#if defined(OS_DETECTION_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
        case IME_TGL:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(KC_RALT);
                    wait_ms(10);
                    tap_code16(KC_GRV);
                    unregister_code16(KC_RALT);
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(G(KC_SPC));
                } else {
                    unregister_code16(G(KC_SPC));
                }
                return false;
            }
            break;
        case ZOOM_IN:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(C(KC_EQL));
                } else {
                    unregister_code16(C(KC_EQL));
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(G(KC_EQL));
                } else {
                    unregister_code16(G(KC_EQL));
                }
                return false;
            }
            break;
        case ZOOM_OUT:
            switch (detected_host_os()) {
            case OS_WINDOWS:
                if (record->event.pressed) {
                    register_code16(C(KC_MINS));
                } else {
                    unregister_code16(C(KC_MINS));
                }
                return false;
            case OS_MACOS:
            default:
                if (record->event.pressed) {
                    register_code16(G(KC_MINS));
                } else {
                    unregister_code16(G(KC_MINS));
                }
                return false;
            }
            break;
#endif
        case KC_BSPC:
            // https://docs.qmk.fm/feature_advanced_keycodes#shift-backspace-for-delete
            {
                // Initialize a boolean variable that keeps track
                // of the delete key status: registered or not?
                static bool delkey_registered;
                uint8_t mod_state = get_mods();
                if (record->event.pressed) {
                    // Detect the activation of either shift keys
                    if (mod_state & MOD_MASK_SHIFT) {
                        // First temporarily canceling both shifts so that
                        // shift isn't applied to the KC_DEL keycode
                        del_mods(MOD_MASK_SHIFT);
                        register_code(KC_DEL);
                        // Update the boolean variable to reflect the status of KC_DEL
                        delkey_registered = true;
                        // Reapplying modifier state so that the held shift key(s)
                        // still work even after having tapped the Backspace/Delete key.
                        set_mods(mod_state);
                        return false;
                    }
                } else { // on release of KC_BSPC
                    // In case KC_DEL is still being sent even after the release of KC_BSPC
                    if (delkey_registered) {
                        unregister_code(KC_DEL);
                        delkey_registered = false;
                        return false;
                    }
                }
            }
            break;
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
        default:
            break;
    }
    return true;
}
