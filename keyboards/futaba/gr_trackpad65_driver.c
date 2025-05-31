/* Copyright 2017 Joshua Broekhuijsen <snipeye+qmk@gmail.com>
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2021 Dasky (@daskygit)
 * Copyright 2024 Geek-rabb1t (@geek-rabb1t)
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
#include "i2c_master.h"
#include "azoteq_iqs5xx.h"
#include "pointing_device_internal.h"
#include "pointing_device.h"
#include "debug.h"
#include "wait.h"
#include "timer.h"
#include "gr_trackpad65_driver.h"
#include <math.h>


trackpad_config_t trackpad_config = {
    .reverse_vertical_scroll = false,
    .reverse_horizontal_scroll = false,
    .disable_3fingers_tap = false,
    .disable_tap = false,
    .scroll_scale_percent = SCROLL_SCALE_PERCENT
};


void read_trackpad_config(void) {
    uint32_t data = eeconfig_read_kb();
    trackpad_config.reverse_vertical_scroll = (data & REVERSE_VERTICAL_SCROLL_MASK) > 0;
    trackpad_config.reverse_horizontal_scroll = (data & REVERSE_HORIZONTAL_SCROLL_MASK) > 0;
    trackpad_config.disable_3fingers_tap = (data & DISABLE_3FINGERS_MASK) > 0;
    trackpad_config.disable_tap = (data & DISABLE_TAP_MASK) > 0;
}

void update_trackpad_config(trackpad_config_t config) {
    uint32_t data = 0;
    data += trackpad_config.reverse_vertical_scroll ? REVERSE_VERTICAL_SCROLL_MASK : 0;
    data += trackpad_config.reverse_horizontal_scroll ? REVERSE_HORIZONTAL_SCROLL_MASK : 0;
    data += trackpad_config.disable_3fingers_tap ? DISABLE_3FINGERS_MASK : 0;
    data += trackpad_config.disable_tap ? DISABLE_TAP_MASK : 0;
    eeconfig_update_kb(data);
}

trackpad_event_t trackpad_event = {
    .type = trackpad_event_none,
    .num_of_fingers = 0
};

static i2c_status_t azoteq_iqs5xx_init_status = 1;

void pointing_device_driver_init(void) {
    i2c_init();
    azoteq_iqs5xx_wake();
    azoteq_iqs5xx_reset_suspend(true, false, true);
    wait_ms(100);
    azoteq_iqs5xx_wake();
    if (azoteq_iqs5xx_get_product() != AZOTEQ_IQS5XX_UNKNOWN) {
        azoteq_iqs5xx_setup_resolution();
        azoteq_iqs5xx_init_status = azoteq_iqs5xx_set_report_rate(AZOTEQ_IQS5XX_REPORT_RATE, AZOTEQ_IQS5XX_ACTIVE, false);
        azoteq_iqs5xx_init_status |= azoteq_iqs5xx_set_event_mode(false, false);
        azoteq_iqs5xx_init_status |= azoteq_iqs5xx_set_reati(true, false);
        azoteq_iqs5xx_init_status |= azoteq_iqs5xx_set_xy_config(false, false, false, true, false);
        azoteq_iqs5xx_init_status |= azoteq_iqs5xx_set_gesture_config(true);
        wait_ms(AZOTEQ_IQS5XX_REPORT_RATE + 1);
        read_trackpad_config();
    }
};

void dispatch_swipe_gesture(int16_t swipe_distance_x, int16_t swipe_distance_y, int8_t num_of_fingers) {
    swipe_distance_x = abs(swipe_distance_x) >= FUTABA_SWIPE_THRESHOLD_PIXEL ? swipe_distance_x : 0;
    swipe_distance_y = abs(swipe_distance_y) >= FUTABA_SWIPE_THRESHOLD_PIXEL ? swipe_distance_y : 0;
    if (swipe_distance_x == 0 && swipe_distance_y == 0) {
        reset_trackpad_event();
        return;
    }

    trackpad_gesture_event_t event = abs(swipe_distance_x) > abs(swipe_distance_y) ?
            (swipe_distance_x > 0 ? trackpad_event_swipe_right : trackpad_event_swipe_left):
            (swipe_distance_y > 0 ? trackpad_event_swipe_down : trackpad_event_swipe_up);

    // pd_dprintf("swipe - %d fingers, direction: %d, x:%d, y:%d.\n",num_of_fingers, event, swipe_distance_x, swipe_distance_y);

    trackpad_event.type = event;
    trackpad_event.num_of_fingers = num_of_fingers;
}

void reset_trackpad_event(void) {
    trackpad_event.type = trackpad_event_none;
    trackpad_event.num_of_fingers = 0;
}

int8_t calc_max_fingers(int8_t current, int8_t previous) {
    return (previous > current) ? previous :current;
}

static uint8_t max_fingers = 0;
static int16_t swipe_distance_x = 0;
static int16_t swipe_distance_y = 0;
static uint16_t tap_interval = 0;
static uint16_t tap_timer = 0;
static uint16_t gesture_timer = 0;
static bool doubleTap = false;

static trackpad_state_t trackpad_state = trackpad_state_idle;
static scroll_direction_t scroll_direction = scroll_direction_tbd;
static position_t scroll_rest = {0};

void reset_gesture_status(void) {
    if (max_fingers != 0) {
        pd_dprintf("reset state.\n");
    }

    max_fingers = 0;
    swipe_distance_x = 0;
    swipe_distance_y = 0;
    doubleTap = false;
    scroll_direction = scroll_direction_tbd;
    scroll_rest.x = 0;
    scroll_rest.y = 0;
}


report_mouse_t idle_strategy(trackpad_base_data_t *trackpad_data) {
    reset_gesture_status();
    report_mouse_t report = {0};
    return report;
}

report_mouse_t touch_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};
    return temp_report;
}


report_mouse_t move_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};
    if (trackpad_data->num_of_fingers  >= 2) {

        int scroll_dir_x = (trackpad_config.reverse_horizontal_scroll) ? -1 : 1;
        int scroll_dir_y = (trackpad_config.reverse_vertical_scroll  ) ? -1 : 1;

        scroll_rest.x += trackpad_data->pos.x * trackpad_config.scroll_scale_percent;
        scroll_rest.y += trackpad_data->pos.y * trackpad_config.scroll_scale_percent;
        int scroll_x = scroll_rest.x / 100;
        int scroll_y = scroll_rest.y / 100;
        scroll_rest.x -= scroll_x * 100;
        scroll_rest.y -= scroll_y * 100;

        // Restrict scroll direction.
        if (scroll_direction == scroll_direction_tbd) {
            if (scroll_x == 0 && scroll_y == 0) {
                return temp_report;
            }
            if (abs(scroll_y) > abs(scroll_x) * 2) {
                scroll_direction = scroll_direction_vertical;
            } else if (abs(scroll_x) > abs(scroll_y) *2) {
                scroll_direction = scroll_direction_horizontal;
            } else {
                scroll_direction = scroll_direction_both;
            }

            return temp_report;
        }

        // Remove restrictions on scroll direction.
        if (abs(trackpad_data->pos.y) > abs(trackpad_data->pos.x) * 2  && abs(trackpad_data->prev_pos.y) > abs(trackpad_data->prev_pos.x) * 2) {
            if (scroll_direction == scroll_direction_horizontal) {
                scroll_direction = scroll_direction_both;
            }

        } else if (abs(trackpad_data->pos.x) > abs(trackpad_data->pos.y) * 2  && abs(trackpad_data->prev_pos.x) > abs(trackpad_data->prev_pos.y) * 2) {
            if (scroll_direction == scroll_direction_vertical) {
                scroll_direction = scroll_direction_both;
            }
        }

        if (scroll_direction == scroll_direction_vertical || scroll_direction == scroll_direction_both) {
            temp_report.v = CONSTRAIN_HID(scroll_y * scroll_dir_y);
        }
        if (scroll_direction == scroll_direction_horizontal || scroll_direction == scroll_direction_both) {
            temp_report.h = CONSTRAIN_HID(scroll_x * scroll_dir_x);
        }


    } else {
        scroll_direction = scroll_direction_tbd;
        temp_report.x = trackpad_data->mouse_report_x;
        temp_report.y = trackpad_data->mouse_report_y;
    }

    return temp_report;
}

report_mouse_t gesture_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};

    swipe_distance_x += trackpad_data->pos.x;
    swipe_distance_y += trackpad_data->pos.y;

    return temp_report;
}

report_mouse_t gesture_fire_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};

    // pd_dprintf("gesture: %d fingers.(%d, %d)\n",max_fingers, swipe_distance_x, swipe_distance_y);
    dispatch_swipe_gesture(swipe_distance_x, swipe_distance_y, max_fingers);

    return temp_report;
}

dispatch_button_t dispatch_buttons(int num_of_fingers) {
    dispatch_button_t temp = {
        .is_pressed = false,
        .button_num = POINTING_DEVICE_BUTTON1
    };

    if ( num_of_fingers > 3 ||
        trackpad_config.disable_tap ||
        (num_of_fingers == 3 && trackpad_config.disable_3fingers_tap)) {
        temp.is_pressed = false;
        return temp;
    }
    temp.is_pressed = true;
    temp.button_num =    (num_of_fingers == 3) ?    POINTING_DEVICE_BUTTON3 :
                        ((num_of_fingers == 2) ?    POINTING_DEVICE_BUTTON2 :
                                                    POINTING_DEVICE_BUTTON1);
    return temp;
}

report_mouse_t press_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};

    dispatch_button_t button = dispatch_buttons(max_fingers);
    if (!button.is_pressed) {
        return temp_report;
    }
    // pd_dprintf("press: %d fingers.\n",max_fingers);
    temp_report.buttons = pointing_device_handle_buttons(temp_report.buttons, true, button.button_num);

    if (trackpad_data->mouse_report_x != 0 || trackpad_data->mouse_report_y != 0) {
        doubleTap = false;
    }

    temp_report.x = trackpad_data->mouse_report_x;
    temp_report.y = trackpad_data->mouse_report_y;

    return temp_report;
}

report_mouse_t wait_strategy(trackpad_base_data_t *trackpad_data) {
    report_mouse_t temp_report = {0};

    if (doubleTap) {
        doubleTap = false;
        return temp_report;
    }
    dispatch_button_t button = dispatch_buttons(max_fingers);
    if (!button.is_pressed) {
        return temp_report;
    }
    // pd_dprintf("press wait: %d fingers.\n",max_fingers);
    temp_report.buttons = pointing_device_handle_buttons(temp_report.buttons, true, button.button_num);

    return temp_report;
}

report_mouse_t (*find_strategy(trackpad_state_t state))(trackpad_base_data_t *) {
    switch (state) {
        case trackpad_state_idle: return idle_strategy;
        case trackpad_state_touch: return touch_strategy;
        case trackpad_state_move: return move_strategy;
        case trackpad_state_gesture: return gesture_strategy;
        case trackpad_state_gesture_fire: return gesture_fire_strategy;
        case trackpad_state_press: return press_strategy;
        case trackpad_state_wait: return wait_strategy;
    }
    return idle_strategy;
}


touch_state_t get_touch_state(trackpad_base_data_t *trackpad_data) {
    if (trackpad_data->touch_strength == 0) {
        return touch_state_none;
    } else if (trackpad_data->touch_strength >= FUTABA_TAP_STRENGTH_THRESHOLD) {
        return touch_state_press;
    }
    return touch_state_touch;

}

trackpad_state_t update_current_state(trackpad_base_data_t *trackpad_data, trackpad_state_t prev_state) {
    touch_state_t touch_state = get_touch_state(trackpad_data);
    // pd_dprintf("touch: %d.\n",touch_state);

    // The state of the trackpad is determined by a combination of the previous state and the current touch state.
    if (prev_state == trackpad_state_idle) {
        if (touch_state == touch_state_touch) {
            tap_timer = timer_read();
            max_fingers = calc_max_fingers(trackpad_data->num_of_fingers, max_fingers);
            return trackpad_state_touch;
        }
        if (touch_state == touch_state_press) {
            max_fingers = calc_max_fingers(trackpad_data->num_of_fingers, max_fingers);
            return trackpad_state_press;
        }
    }

    if (prev_state == trackpad_state_touch) {
        max_fingers = calc_max_fingers(trackpad_data->num_of_fingers, max_fingers);
        if (touch_state == touch_state_none) {
            if (timer_elapsed(tap_timer) <= FUTABA_MAX_TAP_TIME) {
                // pd_dprintf("touch : %d fingers.time: (%d)\n",max_fingers, timer_elapsed(tap_timer));
                return trackpad_state_press;
            }
            return trackpad_state_idle;
        }

        if (trackpad_data->mouse_report_x != 0 || trackpad_data->mouse_report_y != 0) {
            if (max_fingers > 2) {
                // pd_dprintf("start gesture: %d fingers.(x,y): (%d, %d)\n",max_fingers,trackpad_data->mouse_report_x, trackpad_data->mouse_report_y);
                gesture_timer = timer_read();
                return trackpad_state_gesture;
            }
            return trackpad_state_move;
        }

        if (touch_state == touch_state_press) {
            if (timer_elapsed(tap_timer) <= FUTABA_MAX_TAP_TIME) {
                // pd_dprintf("strong touch : %d fingers.time: (%d)\n",max_fingers, timer_elapsed(tap_timer));
                return trackpad_state_press;
            }
        }
    }

    if (prev_state == trackpad_state_move) {
        if (touch_state == touch_state_none) {
            return trackpad_state_idle;
        }
    }

    if (prev_state == trackpad_state_gesture) {
        if (touch_state == touch_state_none) {
            if (timer_elapsed(gesture_timer) <= FUTABA_MAX_GESTURE_ACTIVE_TIME) {
                return trackpad_state_gesture_fire;
            }
            return trackpad_state_idle;
        }
    }

    if (prev_state == trackpad_state_gesture_fire) {
        if (touch_state == touch_state_none) {
            return trackpad_state_idle;
        }
        if (touch_state == touch_state_touch) {
            return trackpad_state_touch;
        }
        if (touch_state == touch_state_press) {
            max_fingers = calc_max_fingers(trackpad_data->num_of_fingers, max_fingers);
            return trackpad_state_press;
        }
    }

    if(prev_state == trackpad_state_press) {
        if (touch_state == touch_state_none) {
            tap_interval = timer_read();
            return trackpad_state_wait;
        }
    }

    if (prev_state == trackpad_state_wait) {
        if (touch_state == touch_state_none) {
            if (timer_elapsed(tap_interval) >= FUTABA_RETAP_WAITING_TIME) {
                return trackpad_state_idle;
            }
        }

        if (touch_state == touch_state_touch || touch_state == touch_state_press) {
            doubleTap = true;
            return trackpad_state_press;
        }
    }

    return prev_state;
}

int get_touch_strength(azoteq_iqs5xx_base_data_t base_data) {
    int fingers[5] = {
        AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_1.touch_strength.h, base_data.finger_1.touch_strength.l),
        AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_2.touch_strength.h, base_data.finger_2.touch_strength.l),
        AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_3.touch_strength.h, base_data.finger_3.touch_strength.l),
        AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_4.touch_strength.h, base_data.finger_4.touch_strength.l),
        AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_5.touch_strength.h, base_data.finger_5.touch_strength.l)
    };
    int max = 0;
    for (int i = 0; i < 5; i++) {
        if (fingers[i] > max) {
            max = fingers[i];
        }
    }
    return max;
}


static position_t prev_positions[5] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
static int cursor_finger_num = 0;
static uint16_t timer;
// 指が 0本から 1本以上に変わった場合、一定サイクルは座標の変更を無視したほうがよさそう。
void get_finger_delta(azoteq_iqs5xx_base_data_t base_data, position_t *delta) {
    position_with_strength_t fingers[5] = {
        {
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_1.absolute_x.h, base_data.finger_1.absolute_x.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_1.absolute_y.h, base_data.finger_1.absolute_y.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_1.touch_strength.h, base_data.finger_1.touch_strength.l),
        },
        {
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_2.absolute_x.h, base_data.finger_2.absolute_x.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_2.absolute_y.h, base_data.finger_2.absolute_y.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_2.touch_strength.h, base_data.finger_2.touch_strength.l),
        },
        {
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_3.absolute_x.h, base_data.finger_3.absolute_x.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_3.absolute_y.h, base_data.finger_3.absolute_y.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_3.touch_strength.h, base_data.finger_3.touch_strength.l),
        },
        {
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_4.absolute_x.h, base_data.finger_4.absolute_x.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_4.absolute_y.h, base_data.finger_4.absolute_y.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_4.touch_strength.h, base_data.finger_4.touch_strength.l),
        },
        {
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_5.absolute_x.h, base_data.finger_5.absolute_x.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_5.absolute_y.h, base_data.finger_5.absolute_y.l),
            AZOTEQ_IQS5XX_COMBINE_H_L_BYTES(base_data.finger_5.touch_strength.h, base_data.finger_5.touch_strength.l),
        },
    };

    position_t deltas[5] = {0};
    bool first_touch = true;
    for (int i = 0; i < 5; i++) {
        if (fingers[i].strength == 0 ) {
            fingers[i].x = -1;
            fingers[i].y = -1;
            prev_positions[i].x = -1;
            prev_positions[i].y = -1;
            continue;
        }

        if (prev_positions[i].x != -1) {
            first_touch = false;
            // Limit the maximum movement.
            int x = fingers[i].x - prev_positions[i].x;
            x = x > 300 ? 300 : x < -300 ? -300 : x;
            deltas[i].x = x;

            int y = fingers[i].y - prev_positions[i].y;
            y = y > 300 ? 300 : y < -300 ? -300 : y;
            deltas[i].y = y;
        }

        prev_positions[i].x = fingers[i].x;
        prev_positions[i].y = fingers[i].y;
    }

    if (base_data.number_of_fingers > 0 && first_touch) {
        timer = timer_read();
    }

    if (base_data.number_of_fingers == 1 && timer_elapsed(timer) < WAIT_TIME_FOR_CURSOR_MOVEMENT) {
        delta->x = 0;
        delta->y = 0;
        return;
    }

    if (base_data.number_of_fingers >= 2 && timer_elapsed(timer) < WAIT_TIME_FOR_MULTI_TAP_CURSOR_MOVEMENT) {
        delta->x = 0;
        delta->y = 0;
        return;
    }

    // 前回のサイクルでカーソル移動に使った指を優先する。
    if (cursor_finger_num != -1 && (abs(deltas[cursor_finger_num].x) + abs(deltas[cursor_finger_num].y) > 2)) {
        delta->x = deltas[cursor_finger_num].x;
        delta->y = deltas[cursor_finger_num].y;
        return;
    }

    // カーソル飛び対策として、指が変わった直後は移動量を無視する。
    for (int i = 0; i < 5; i++) {
        if (abs(deltas[i].x) + abs(deltas[i].y) > 2) {
            delta->x = 0;
            delta->y = 0;
            cursor_finger_num = i;
            return;
        }
    }

    delta->x = 0;
    delta->y = 0;
    cursor_finger_num = -1;

    return;
}

mouse_xy_report_t correct_cursor(int delta, int prev, bool print) {

    int avg = (delta + prev);
    int ratio = (fmin(abs(avg), 255)) * 15 / 255 + 5;
    int mov = avg * ratio / 20;

    return (mouse_xy_report_t) CONSTRAIN_HID_XY((int)mov);
}

static position_t prev = {0};

report_mouse_t pointing_device_generate_report(azoteq_iqs5xx_base_data_t base_data) {

    position_t position = {0};
    get_finger_delta(base_data, &position);

    trackpad_base_data_t trackpad_data = {
        .pos.x = position.x,
        .pos.y = position.y,
        .prev_pos.x = prev.x,
        .prev_pos.y = prev.y,
        .mouse_report_x = correct_cursor(position.x, prev.x, true),
        .mouse_report_y = correct_cursor(position.y, prev.y, false),
        .touch_strength = get_touch_strength(base_data),
        .num_of_fingers = base_data.number_of_fingers,
    };

    prev.x = position.x;
    prev.y = position.y;

    trackpad_state = update_current_state(&trackpad_data, trackpad_state);
    return find_strategy(trackpad_state)(&trackpad_data);
}


report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    report_mouse_t temp_report           = {0};
    static uint8_t previous_button_state = 0;
    static uint8_t read_error_count      = 0;

    if (azoteq_iqs5xx_init_status == I2C_STATUS_SUCCESS) {
        azoteq_iqs5xx_base_data_t base_data = {0};
        i2c_status_t status = azoteq_iqs5xx_get_base_data(&base_data);

        if (status == I2C_STATUS_SUCCESS) {
            read_error_count = 0;
            temp_report = pointing_device_generate_report(base_data);
            previous_button_state = temp_report.buttons;

        } else {
            if (read_error_count > 10) {
                read_error_count      = 0;
                previous_button_state = 0;
            } else {
                read_error_count++;
            }
            temp_report.buttons = previous_button_state;
        }
    } else {
        pd_dprintf("IQS5XX - Init failed: %d \n", azoteq_iqs5xx_init_status);
    }

    return temp_report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return azoteq_iqs5xx_get_cpi();
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    azoteq_iqs5xx_set_cpi(cpi);
}
