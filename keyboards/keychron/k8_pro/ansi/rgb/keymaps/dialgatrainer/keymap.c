/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H



enum layers{
  HOME_BASE,
  HOME_FN,
  WORK_BASE,
  WORK_FN
};

enum custom_keycodes {
    WK_PASSWD = NEW_SAFE_RANGE,
};





// socd_cleaner_t socd_opposing_pairs[] = {
//   {{KC_W, KC_S}, SOCD_CLEANER_LAST},
//   {{KC_A, KC_D}, SOCD_CLEANER_LAST},
// };


//**************** SOME GLOBALS *********************//

// Config
const float ledDimRatio = 0.50;
bool isRecording = false;
static uint16_t recording_blink_cycle_timer;
static uint16_t recording_fade_in_timer;
static uint16_t recording_fade_out_timer;
static uint16_t recording_hsv_value;
static uint8_t fade_in_step_counter;
static uint8_t fade_out_step_counter;
static uint8_t recording_r, recording_g, recording_b;
static uint8_t target_key = 0;


// Declare tap dance identifiers
enum {
    AT_EMAIL,
    DM_ESC
};

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;


// static uint8_t target_key = 50; // caps lock


// clang-format off


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[HOME_BASE] = LAYOUT_tkl_ansi(
     TD(DM_ESC),   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   KC_CTANA, UG_NEXT,
     KC_GRV,   KC_1, TD(AT_EMAIL), KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
MT(MOD_LCTL,KC_ESC),KC_A,KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                              KC_SPC,                               KC_RALT,  KC_RGUI, MO(HOME_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[HOME_FN] = LAYOUT_tkl_ansi(
     _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            _______,  _______,  UG_TOGG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,            _______,
     _______,  _______,  _______,                              _______,                              _______,  _______,  _______,  _______,  _______,  _______,  _______),

[WORK_BASE] = LAYOUT_tkl_ansi(
     TD(DM_ESC),   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   KC_CTANA, UG_NEXT,
     KC_GRV,   KC_1, TD(AT_EMAIL), KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
MT(MOD_LCTL,KC_ESC),KC_A,KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                              KC_SPC,                               KC_RALT,  KC_RGUI, MO(WORK_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WORK_FN] = LAYOUT_tkl_ansi(
     _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            _______,  _______,  UG_TOGG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     DM_REC1,  DM_REC2,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,            _______,
     DM_PLY1,  DM_PLY2,  _______,                              _______,                              _______,  _______,  _______,  _______,  _______,  _______,  _______)

};



// clang-format on

bool is_work_layer(layer_state_t state) {
    return get_highest_layer(state) > WORK_BASE? true: false;
}



// socd_cleaner_t socd_opposing_pairs[] = {
//   {{KC_W, KC_S}, SOCD_CLEANER_LAST},
//   {{KC_A, KC_D}, SOCD_CLEANER_LAST},
// };




bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WK_PASSWD:
            if (record->event.pressed) {
                SEND_STRING("Abc123!!!");
            }
            return false;
            break;
    }
    return true;
}


// void sentence_case_primed(bool primed) {
//  // Change B0 to the pin for the LED to use.
//   writePin(LED_CAPS_LOCK_PIN, primed);
// }


//**************** HELPER FUNCTIONS *****************//

// I am unable to pass RGB[3] array pointer correctly so I get R, G, & B values like this instead
uint8_t get_recording_red_rgb(uint8_t rgb, uint16_t value)
{
    HSV hsvRed = {0, 255, value * ledDimRatio};
    RGB rgbRed = hsv_to_rgb(hsvRed);
    const uint8_t RED_HSV[3] = {rgbRed.r, rgbRed.g, rgbRed.b};
    switch (rgb) {
        case 0:
            return RED_HSV[0];
            break;
        case 1:
            return RED_HSV[1];
            break;
        case 2:
            return RED_HSV[2];
            break;
        default:
            return false;
    }
}

// I am unable to pass RGB[3] array pointer correctly so I set R, G, & B values like this instead
void set_recording_rgb(uint16_t value)
{
    recording_r = get_recording_red_rgb(0, value);
    recording_g = get_recording_red_rgb(1, value);
    recording_b = get_recording_red_rgb(2, value);
}

void reset_recording_blink(void)
{
    recording_blink_cycle_timer = timer_read();
    recording_fade_in_timer = timer_read();
    recording_fade_out_timer = timer_read();
    recording_hsv_value = 0;
    fade_in_step_counter = 0;
    fade_out_step_counter = 0;
}


//**************** MATRIX SCANS *********************//

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{


    //****LAYER INDICATOR *******************************/

    if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_ORANGE);
                }
            }
        }
        return true;
    }



    // Blinking led indicator when recording Dynamic Macro with fading in/out
    if (isRecording) {

        const uint16_t static_on_time = 500;
        const uint16_t static_off_time = 500;
        const uint8_t fade_timing = 100;
        const uint8_t fade_step = 10;
        const uint8_t fade_value_decrement_size = rgb_matrix_config.hsv.v / fade_step;
        const uint8_t fade_cycle_time_elapsed = fade_timing / fade_step;

        // Blink cycle
        if (timer_elapsed(recording_blink_cycle_timer) < static_on_time) {
            if (timer_elapsed(recording_fade_in_timer) > fade_cycle_time_elapsed && fade_in_step_counter < fade_step) {
                recording_hsv_value = recording_hsv_value + fade_value_decrement_size;
                set_recording_rgb(recording_hsv_value);
                fade_in_step_counter = fade_in_step_counter + 1;
                recording_fade_in_timer = timer_read();
            }
        } else {
            if (timer_elapsed(recording_fade_out_timer) > fade_cycle_time_elapsed && fade_out_step_counter < fade_step) {
                recording_hsv_value = recording_hsv_value - fade_value_decrement_size;
                set_recording_rgb(recording_hsv_value);
                fade_out_step_counter = fade_out_step_counter + 1;
                recording_fade_out_timer = timer_read();
            }
        }

        // Reset blink cycle after each fade-in/fade-out cycle
        if (timer_elapsed(recording_blink_cycle_timer) > static_on_time + static_off_time) {
            reset_recording_blink();
        }
        rgb_matrix_set_color(target_key, recording_r, recording_g, recording_b);
        rgb_matrix_set_color(63, recording_r, recording_g, recording_b);
        rgb_matrix_set_color(76, recording_r, recording_g, recording_b);
        rgb_matrix_set_color(64, recording_r, recording_g, recording_b);
        rgb_matrix_set_color(77, recording_r, recording_g, recording_b);
    }
    return true;


}

//**************** DYNAMIC MACRO *********************//

// Triggered when you start recording a macro.
bool dynamic_macro_record_start_user(int8_t direction) {
    isRecording = true;
    reset_recording_blink();
    return true;
}

// Triggered when the macro recording is stopped.
bool dynamic_macro_record_end_user(int8_t direction) {
    isRecording = false;
    return true;
}


// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void email_ql_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_2);
            break;
        case TD_SINGLE_HOLD:
            SEND_STRING(is_work_layer(layer_state)? "olivia@cj-tech.uk": "oliviabarker900@gmail.com");
            break;
        default:
            break;
    }
}


void escape_ql_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
        tap_code(KC_ESC);
        break;
        case TD_DOUBLE_TAP:
        dynamic_macro_stop_recording();
        tap_code(KC_ESC);
            break;
        default:
            break;
    }
}

void _ql_reset(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = TD_NONE;
}

// swap out the reset function for a specific one if needed

// void email_ql_reset(tap_dance_state_t *state, void *user_data) {
//    // If the key was held down and now is released then switch off the layer
//    // if (ql_tap_state.state == TD_SINGLE_HOLD) {
//        //
//    // }
    // _ql_reset(state, user_data);
// }



// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [AT_EMAIL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, email_ql_finished, _ql_reset),
    [DM_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, escape_ql_finished, _ql_reset)
};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 250;
        default:
            return TAPPING_TERM;
    }
}
