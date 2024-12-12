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

#include "lotus58.h"

#ifdef RGB_MATRIX_ENABLE

#define LED_MATRIX_CENTER {110, 21}

 led_config_t g_led_config = {
  {
        // left hand switch matrix to led map
        {  5,  4,  3,  2,   1,   0 },
        {  6,  7,  8,  9,  10,  11 },
        {  17, 16, 15, 14,  13, 12 },
        {  18, 19, 20, 21, 22,  23 },
        {  NO_LED, 28, 27, 26, 25, 24 },

        // right hand switch matrix to led map
        {  40, 39, 38, 37,  36,  35 },
        {  41, 42, 43, 44,  45,  46 },
        {  52, 51, 50, 49,  48,  47 },
        {  53, 54, 55, 56,  57,  58 },
        {  NO_LED, 63, 62, 61, 60, 59 }
    },
    {
        { 55,  2}, { 44,  1}, { 33,  0}, { 22,  1}, { 11,  3}, {  0,  4},
        {  0, 14}, { 11, 13}, { 22, 11}, { 33, 10}, { 44, 11}, { 55, 12},
        { 55, 22}, { 44, 21}, { 33, 20}, { 22, 21}, { 11, 23}, {  0, 24},
        {  0, 34}, { 11, 33}, { 22, 31}, { 33, 30}, { 44, 31}, { 55, 32},
        { 70, 27}, { 66, 40}, { 50, 42}, { 39, 41} ,{ 28, 40},

        { 34, 40}, { 63, 31}, { 6, 33},
        { 4, 4},   { 29, 1},  { 49, 3},

        {165, 2}, {176, 1}, {187, 0}, {198, 1}, {209, 3}, {220, 4},
        {220, 14}, {209, 13}, {198, 11}, {187, 10}, {176, 11}, {165, 12},
        {165, 22}, {176, 21}, {187, 20}, {198, 21}, {209, 23}, {220, 24},
        {220, 34}, {209, 33}, {198, 31}, {187, 30}, {176, 31}, {165, 32},
        {150, 27}, {154, 40}, {170, 42}, {181, 41}, {192, 40},

        {187, 40}, {155, 31}, {215, 33},
        {215, 4},  {193, 1},  {171, 3}
    },
    // led flags
    {
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,

        LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,
        LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,

        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,

        LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,
        LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW
    }
 };

void suspend_power_down_kb(void) {
    rgb_matrix_set_suspend_state(true);
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    rgb_matrix_set_suspend_state(false);
    suspend_wakeup_init_user();
}
#endif
