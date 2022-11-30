/* Copyright 2021-2022 TweetyDaBird, Marius Renner
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
#include "eeconfig.h"

#define _QWERTY 0
#define _GAME 1
#define _LOWER 2
#define _RAISE 3
#define _LOWERGAME 4
#define _ADJUST 7

// Simple custom keycodes
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  GAME,
  LOWER,
  RAISE,
  LOWERGAME,
  LOWERED_GUI // GUI + LOWER, especially useful for GUI+Number shortcuts
};

// Tap Dances START

// Tap Dance Keycodes
enum tap_dances {
    TD_MEDIA,
    TD_HRESET, // Hold reset
};

// Tap Dance states
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_DOUBLE_HOLD
} td_state_t;

typedef struct {
    bool       did_enable_layer;
    td_state_t state;
} td_tap_t;

// Determine the tapdance state to return
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (!state->pressed) return TD_DOUBLE_SINGLE_TAP;
        else return TD_DOUBLE_HOLD;
    }

    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

void hreset_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state_t tap_state = cur_dance(state);
    switch (tap_state) {
        case TD_SINGLE_TAP:
            break;
        case TD_SINGLE_HOLD:
            reset_keyboard();
            break;
        case TD_DOUBLE_SINGLE_TAP:
        case TD_DOUBLE_HOLD:
        default:
            break;
    }
}

void media_tap_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state_t tap_state = cur_dance(state);
    switch (tap_state) {
        case TD_SINGLE_TAP:
            tap_code(KC_MPLY);
            break;
        case TD_SINGLE_HOLD:
            tap_code(KC_MPRV);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            tap_code(KC_MNXT);
            break;
        case TD_DOUBLE_HOLD:
            break;
        default:
            break;
    }
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_MEDIA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, media_tap_finished, NULL),
    [TD_HRESET] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, hreset_finished, NULL),
};

// Tap Dances END

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Delayed reset so I stop pressing it by accident
        case TD(TD_HRESET):
            return 2000;
        default:
            return TAPPING_TERM;
    }
}

#define CK_LALT MT(MOD_LALT, KC_RGUI)
#define CK_RALT MT(MOD_RALT, KC_APP)

/* Keymap for reference
 * The outermost key on the last row is the encoder press!
 * ,-----------------------------------------.                    ,------------------------------------------.
 * |  ESC |  1   |   2  |   3  |   4  |   5  |   ___       ____   |   6  |  7   |   8  |   9  |  0   |   `   |
 * |------+------+------+------+------+------|  /   \     |    |  |------+------+------+------+------+-------|
 * | Tab  |   Q  |   W  |   F  |   P  |   B  | ( ENC )    |OLED|  |   J  |   L  |   U  |   Y  |   ;  |   -   |
 * |------+------+------+------+------+------|  \___/     |____|  |------+------+------+------+------+-------|
 * | Ctrl |   A  |   R  |   S  |   T  |   G  |                    |   M  |   N  |   E  |   I  |   O  |   '   |
 * |------+------+------+------+------+------|-------.    ,-------|------+------+------+------+------+-------|
 * | Shift|   Z  |   X  |   C  |   D  |   V  |   {   |    |   }   |   K  |   H  |   ,  |   .  |   /  | Shift |
 * `-----------------------------------------|-------/    \-------|------------------------------------------'
 *                     | Alt | Del | Raise / Space /      \ Enter \ Lower | Bksp | Alt  |
 *                     | Gui |     |      |       /        \       |      |      | Menu |
 *                     `-----'-----'------'-------'         '------'------'------'------'
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(
    KC_ESC,  KC_1,  KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,  KC_7,    KC_8,     KC_9,     KC_0,     KC_GRV,
    KC_TAB,  KC_Q,  KC_W,    KC_F,    KC_P,    KC_B,                        KC_J,  KC_L,    KC_U,     KC_Y,     KC_SCLN,  KC_MINS,
    KC_LCTL, KC_A,  KC_R,    KC_S,    KC_T,    KC_G,                        KC_M,  KC_N,    KC_E,     KC_I,     KC_O,     KC_QUOT,
    KC_LSFT, KC_Z,  KC_X,    KC_C,    KC_D,    KC_V, KC_LBRC,     KC_RBRC,  KC_K,  KC_H,    KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
        TD(TD_MEDIA), CK_LALT, KC_DEL,  RAISE, KC_SPC,            KC_ENT,   LOWER, KC_BSPC, CK_RALT,  XXXXXXX
),

[_GAME] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,      KC_8,    KC_9,   KC_0,    KC_GRV,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,      KC_I,    KC_O,     KC_P,     KC_MINS,
    KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,      KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,     KC_RBRC,  KC_N,    KC_M,      KC_COMM, KC_DOT,   KC_SLSH,  KC_RSFT,
        TD(TD_MEDIA), CK_LALT, KC_DEL,  RAISE, KC_SPC,            KC_ENT,   LOWER, KC_BSPC, CK_RALT,  XXXXXXX
),

[_LOWER] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
   KC_GRV, KC_1   , KC_2   , KC_3   , KC_4,    KC_5   ,                         KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
  _______, KC_F1  , KC_F2  , KC_F3  , KC_F4,   KC_F5,                           KC_F6  , KC_4   , KC_5   , KC_6   , KC_DOT , KC_ASTR,
  _______, KC_F7  , KC_F8  , KC_F9  , KC_F10,  KC_F11, _______,        KC_0,    KC_F12 , KC_1   , KC_2   , KC_3   , KC_0   , KC_PLUS,
                    _______, _______, _______, _______,  _______,       _______, _______, _______, _______, _______
),

[_LOWERGAME] = LAYOUT(
  _______, KC_6   , KC_7   , KC_8   , KC_9   , KC_0   ,                         _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,   _______,                       _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,  _______, _______,        _______,    _______, _______, _______, _______, _______, _______,
                    _______, _______, _______, _______,  _______,       _______, _______, _______, _______, _______
),

[_RAISE] = LAYOUT(
  _______, _______, _______,  _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
  _______, _______, KC_LCBR,  KC_RCBR, KC_EQL, _______,                          KC_LCBR, KC_RCBR, KC_LPRN, KC_RPRN, _______, KC_DEL,
  KC_DEL, KC_PAUSE, _______,  KC_PGUP, KC_HOME, KC_RBRC,                         KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_PIPE, KC_GRV,
  _______, KC_PSCR, KC_PAST,  KC_PGDN,  KC_END,  KC_EQL, KC_RWIN,       KC_APP, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, _______,
                     _______,  _______, _______, _______, _______,       KC_BSPC, _______, _______, _______, _______
),

[_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     RGB_TOG, RGB_MOD, RGB_VAI, RGB_VAD, RGB_HUI, RGB_HUD,
  XXXXXXX ,QWERTY,   XXXXXXX,XXXXXXX,TD(TD_HRESET),XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  KC_CAPS, GAME,XXXXXXX, XXXXXXX,    KC_MPLY,  KC_MPRV,                     KC_MNXT, KC_MPLY, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_MPRV,  KC_MNXT, XXXXXXX,     KC_MPRV, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                   _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
  )
};

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_scroll_set_speed(6);
    // I only have an OLED on the left side
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    // 'lotus', 32x32px
    static const char PROGMEM lotus_picture[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x70,
        0x70, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc8, 0xff, 0xe3, 0x03, 0x03, 0x06, 0xfe, 0xff, 0x07, 0x01, 0x00,
        0x00, 0x01, 0x07, 0xff, 0xfe, 0x06, 0x03, 0x03, 0xe3, 0xff, 0xc8, 0xc0, 0xc0, 0xc0, 0xc0, 0x80,
        0x80, 0xc3, 0xef, 0x7c, 0x78, 0x70, 0xe1, 0xc7, 0xcf, 0x9c, 0xb8, 0xf3, 0xff, 0xfc, 0xf0, 0xc0,
        0xc0, 0xf0, 0xfc, 0xff, 0xf3, 0xb8, 0x9c, 0xcf, 0xc7, 0xe1, 0x70, 0x78, 0x7c, 0xef, 0xc3, 0x80,
        0x01, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x07, 0x07, 0x03, 0x03,
        0x03, 0x03, 0x07, 0x07, 0x0d, 0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03, 0x01
    };

    oled_write_raw_P(lotus_picture, sizeof(lotus_picture));
    oled_set_cursor(0, 7);
    oled_write_ln_P(PSTR("Lotus "), false);
    oled_write_ln_P(PSTR("  58  "), false);
    oled_write_ln_P(PSTR(" Glow "), false);
    oled_set_cursor(0, 14);
    switch (biton32(default_layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("DEF"), false);
            break;
        case _GAME:
            oled_write_ln_P(PSTR("GAME"), false);
            break;
    }
    return false;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);

        #ifdef OLED_ENABLE
            oled_clear();
        #endif
      }
      return false;
      break;

    case GAME:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_GAME);

        #ifdef OLED_ENABLE
            oled_clear();
        #endif
      }
      return false;
      break;

    case LOWER:
        if (record->event.pressed) {
          layer_on(_LOWER);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        } else {
          layer_off(_LOWER);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        }
      return false;
      break;

    case LOWERGAME:
        if (record->event.pressed) {
          layer_on(_LOWER);
          layer_on(_LOWERGAME);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        } else {
          layer_off(_LOWER);
          layer_off(_LOWERGAME);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        }
      return false;
      break;

    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    }
  return true;
}

void rgb_matrix_indicators_kb(void) {
    switch(get_highest_layer(layer_state | default_layer_state)) {
        case _LOWER:
            // Turn all underglow flagged leds blue
            for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                    rgb_matrix_set_color(i, RGB_BLUE);
                }
            }
            break;
        case _RAISE:
            // Turn all underglow flagged leds red
            for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                    rgb_matrix_set_color(i, RGB_RED);
                }
            }
            break;
        case _ADJUST:
            // Turn all underglow flagged leds purple
            for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                    rgb_matrix_set_color(i, RGB_PURPLE);
                }
            }
            switch (biton32(default_layer_state)) {
                case _QWERTY:
                    rgb_matrix_set_color(7, RGB_GREEN);
                    rgb_matrix_set_color(16, RGB_BLUE);
                    break;
                case _GAME:
                    rgb_matrix_set_color(7, RGB_BLUE);
                    rgb_matrix_set_color(16, RGB_GREEN);
                    break;
            }
            break;
        default:
            // Turn all underglow flagged leds teal
            for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                    rgb_matrix_set_color(i, RGB_TEAL);
                }
            }
            break;
    }
}

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        // if upper layer is active, volume down
        if (layer_state_is(_RAISE)) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_MS_WH_DOWN);
        }
    } else {
        // if upper layer is active, volume up
        if (layer_state_is(_RAISE)) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_MS_WH_UP);
        }
    }
    return true;
}

#endif
