/* Copyright 2024 Geek-rabb1t (@geek-rabb1t)
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

typedef struct {
    int x;
    int y;
} position_t;

typedef struct {
    int x;
    int y;
    int strength;
} position_with_strength_t;

typedef enum {
    scroll_direction_vertical,
    scroll_direction_horizontal,
    scroll_direction_both,
    scroll_direction_tbd,
} scroll_direction_t;

typedef struct {
    position_t pos;
    position_t prev_pos;
    mouse_xy_report_t mouse_report_x;
    mouse_xy_report_t mouse_report_y;
    int touch_strength;
    int num_of_fingers;
} trackpad_base_data_t;


typedef enum {
    touch_state_none,
    touch_state_touch,
    touch_state_press,
} touch_state_t;

typedef enum {
    trackpad_state_idle,
    trackpad_state_touch,
    trackpad_state_move,
    trackpad_state_gesture,
    trackpad_state_gesture_fire,
    trackpad_state_press,
    trackpad_state_wait,
} trackpad_state_t;

typedef enum  {
    trackpad_event_none = -1,
    trackpad_event_tap = 0,
    trackpad_event_swipe_up = 1,
    trackpad_event_swipe_right = 2,
    trackpad_event_swipe_down = 3,
    trackpad_event_swipe_left = 4,
} trackpad_gesture_event_t;

typedef struct {
    bool is_pressed;
    pointing_device_buttons_t button_num;
} dispatch_button_t;

typedef struct {
    trackpad_gesture_event_t type;
    int8_t num_of_fingers;
} trackpad_event_t;

extern trackpad_event_t trackpad_event;

typedef struct {
    bool    reverse_vertical_scroll;
    bool    reverse_horizontal_scroll;
    bool    disable_3fingers_tap;
    bool    disable_tap;
    uint8_t scroll_scale_percent;
} trackpad_config_t;

#define REVERSE_VERTICAL_SCROLL_MASK   0b0000000000000001
#define REVERSE_HORIZONTAL_SCROLL_MASK 0b0000000000000010
#define DISABLE_3FINGERS_MASK          0b0000000000000100
#define DISABLE_TAP_MASK               0b0000000000001000

extern trackpad_config_t trackpad_config;

const static int8_t FUTABA_SWIPE_THRESHOLD_PIXEL = 20;
const static uint16_t FUTABA_MAX_GESTURE_ACTIVE_TIME = 1000;
const static uint16_t FUTABA_RETAP_WAITING_TIME = 300;
 // 必ずpressにする閾値
const static uint16_t FUTABA_TAP_STRENGTH_THRESHOLD = 850;
// 弱いタッチでもクリックにする時間の最大値
const static uint16_t FUTABA_MAX_TAP_TIME = 200;

const static int WAIT_TIME_FOR_CURSOR_MOVEMENT = 100;
const static int WAIT_TIME_FOR_MULTI_TAP_CURSOR_MOVEMENT = 200;

#ifndef SCROLL_SCALE_PERCENT
#define SCROLL_SCALE_PERCENT 5
#endif

void reset_trackpad_event(void);

void update_trackpad_config(trackpad_config_t config);
