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
    FN,
    WORK_BASE,
    ALT
};

enum custom_keycodes {
    LAYER = NEW_SAFE_RANGE,
    WORK,
    ALTL,
    HOME,
    STP_ESC
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
// static uint8_t target_key = 50; // caps lock


// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[HOME_BASE] = LAYOUT_tkl_ansi(
     STP_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   TT(ALT), RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
MT(MOD_LCTL,KC_ESC),KC_A,KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                              KC_SPC,                                 KC_RALT,  KC_RGUI, MO(FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[FN] = LAYOUT_tkl_ansi(
     KC_TRNS,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                              KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WORK_BASE] = LAYOUT_tkl_ansi(
     STP_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   TT(ALT), RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
MT(MOD_LCTL,KC_ESC),KC_A,KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                              KC_SPC,                                 KC_RALT,  KC_RGUI, MO(FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[ALT] = LAYOUT_tkl_ansi(
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  DM_REC1,  DM_REC2,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  DM_PLY1,  DM_PLY2,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS, KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                              KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),
};



// clang-format on


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ALTL:
            if (record->event.pressed) {
                SEND_STRING("alt");
            }
            return false;
            break;
        case WORK:
            if (record->event.pressed) {
                SEND_STRING("work");
            }
            return false;
            break;
        case HOME:
            if (record->event.pressed) {
                SEND_STRING("HOME");
            }
            return false;
            break;
        case STP_ESC:
            if (record->event.pressed) {
                bool isShifted = get_mods() & MOD_MASK_SHIFT;
                if (isRecording && !isShifted) {

                } else {
                    register_code(KC_ESC);
                }
            }
            return false;
            break;
    }

    return true;
}





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
        // return false;
    }

     if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                }
            }
        }
        return false;
    }
    return true;
}

//**************** DYNAMIC MACRO *********************//

// Triggered when you start recording a macro.
void dynamic_macro_record_start_user(int8_t direction) {
    isRecording = true;
    reset_recording_blink();
}

// Triggered when the macro recording is stopped.
void dynamic_macro_record_end_user(int8_t direction) {
    isRecording = false;

}
