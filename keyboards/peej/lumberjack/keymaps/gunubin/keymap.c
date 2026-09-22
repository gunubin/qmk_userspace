/* Copyright 2020 Paul James
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
#include "recent_keys.h"
#include "sequential_combo.h"

enum layers {
  _QWERTY = 0,
  _FUNCTION,
  _NUMBER,
};

// 謎の不具合でlguiとlaltが入れ替わった!?
#define A_SPC LALT_T(KC_SPC)
#define G_TAB LGUI_T(KC_TAB)

#define A_BSPC RALT_T(KC_BSPC)
#define L1_BSPC LT(1, KC_BSPC)

#define L2_DEL LT(2, KC_DEL)

#define C_A LCTL_T(KC_A)
#define C_ENT LCTL_T(KC_ENT)

#define S_Z LSFT_T(KC_Z)
#define S_SLSH LSFT_T(KC_SLSH)

#define LM_NUM LM(2, MOD_LGUI)
#define S_MINUS LSFT_T(KC_MINUS)

const uint16_t PROGMEM sleep_combo[] = {KC_F14, KC_F15, COMBO_END};
// const uint16_t PROGMEM one_shot_shift_combo[] = {KC_F, KC_J, COMBO_END};
combo_t key_combos[] = {
    COMBO(sleep_combo, KC_NO), // combo index 0
//     COMBO(one_shot_shift_combo, OSM(MOD_LSFT)),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case 0:
            if (pressed) {
                // option + cmd + backspace (system sleep)
                register_code16(RALT(KC_LGUI));
                tap_code(KC_BSPC);
                unregister_code16(RALT(KC_LGUI));
            }
            break;
    }
}

static const sequential_combo_entry_t my_combos[] = {
    {LSFT(KC_9), LSFT(KC_9), LSFT(KC_0)}, // double tap "(" to ")"
    {KC_LCBR, KC_LCBR, KC_RCBR}, // double tap "{" to "}"
    {KC_LBRC, KC_LBRC, KC_RBRC}, // double tap "[" to "]"
    {LSFT(KC_COMM), LSFT(KC_COMM), LSFT(KC_DOT)}, // double tap "<" to ">"
    {KC_DOT, KC_COMM, KC_SCLN}, // tap "." "," to ";"
    {KC_COMM, KC_DOT, KC_EQL}, // tap "," "." to "="
    {KC_COMM, KC_COMM, LSFT(KC_SCLN)}, // double tap "," to ":"
    {S_Z, S_SLSH, LSFT(KC_SLSH)}, // tap "z" "/" to "?"
    // {KC_QUOT, KC_QUOT, KC_DQT}, // tap "z" "/" to "?"
};

void keyboard_post_init_user(void) {
    init_sequential_combos(my_combos, sizeof(my_combos) / sizeof(my_combos[0]));
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty レイヤー
 * ,-----------------------------------------. ,-----------------------------------------.
 * |  1   |  2   |  3   |  4   |  5   | F14  | | F15  |  6   |  7   |  8   |  9   |  0   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |  Q   |  W   |  E   |  R   |  T   |      | |      |  Y   |  U   |  I   |  O   |  P   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * | C_A  |  S   |  D   |  F   |  G   |      | |      |  H   |  J   |  K   |  L   |C_ENT |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * | S_Z  |  X   |  C   |  V   |  B   |      | |      |  N   |  M   |  ,   |  .   |S_SLSH|
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |      | LALT |G_TAB |A_SPC |      | SPC  | |A_BSPC|     |L1_BSPC| OSM_LSFT|   |      |
 * `-----------------------------------------' `-----------------------------------------'
 */
 [_QWERTY] = LAYOUT_ortho_5x12(
    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, KC_F14,   KC_F15, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, _______, _______, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    C_A,     KC_S,    KC_D,    KC_F,    KC_G, _______, _______, KC_H,    KC_J,    KC_K,    KC_L,    C_ENT,
    S_Z,     KC_X,    KC_C,    KC_V,    KC_B, _______, _______, KC_N,    KC_M, KC_COMM,   KC_DOT,   S_SLSH,
    _______,_______, G_TAB,   A_SPC, _______,  LM_NUM, KC_RSFT,_______, L1_BSPC,   L2_DEL,  _______,  _______
),

/* Function レイヤー
 * ,-----------------------------------------. ,-----------------------------------------.
 * |  !   |  @   |  #   |  $   |  %   |      | |      |  ^   |  &   |  *   |  (   |  )   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |  1   |  2   |  3   |  4   |  5   |      | |      |  6   |  7   |  8   |  9   |  0   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |  +   |  (   |  )   |  =   |  :   |      | |      | Bksp |  +   |  -   |  :   |  ;   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |  \   |  {   |  }   |  '   |  "   |      | |      |  |   |  _   |  [   |  ]   |  -   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |      |      | Bksp | Space|      |      | |      |      | Space|      |      |      |
 * `-----------------------------------------' `-----------------------------------------'
 */
 [_FUNCTION] = LAYOUT_ortho_5x12(
    KC_1,     KC_2,      KC_3,   KC_4,     KC_5, _______, _______, KC_6,       KC_7,    KC_8,    KC_9,    KC_0,
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR,  KC_GRV, KC_TILD,
    KC_PLUS,LSFT(KC_9),KC_LBRC,KC_MINS,   KC_EQL, _______, _______, KC_LEFT,KC_DOWN,   KC_UP,KC_RIGHT, KC_SCLN,
    KC_DQT,KC_LCBR,LSFT(KC_COMM), KC_QUOT, KC_UNDS,  _______, _______, KC_PIPE, KC_UNDS, LSFT(KC_COMM), LSFT(KC_DOT), KC_BSLS,
    _______, _______, KC_LGUI, KC_LALT,  _______, _______, _______, _______,  KC_SPC, _______, _______, _______
),

/*
 * Number レイヤー
 * ,-----------------------------------------. ,-----------------------------------------.
 * |  !   |  @   |  #   |  $   |  %   |      | |      |  ^   |  &   |  *   |  (   |  )   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |  1   |  2   |  3   |  4   |  5   |      | |      |  6   |  7   |  8   |  9   |  0   |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |      |      |      |      |      |      | |      |      |      |      |      |      |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |      |      |      |      |      |      | |      |      |      |      |      |      |
 * |------+------+------+------+------+------| |------+------+------+------+------+------|
 * |      |      |      |      |      |      | |      |      |      |      |      |      |
 * `-----------------------------------------' `-----------------------------------------'
 */
 [_NUMBER] = LAYOUT_ortho_5x12(
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
    KC_1,     KC_2,      KC_3,   KC_4,     KC_5, _______, _______, KC_6,       KC_7,    KC_8,    KC_9,    KC_0,
    _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______
)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (update_recent_keys(keycode, record)) {
        //// Expand "qem" to my email address.
        //if (recent[RECENT_SIZE - 3] == KC_Q &&
        //    recent[RECENT_SIZE - 2] == KC_E &&
        //    recent[RECENT_SIZE - 1] == KC_M) {
        //        SEND_STRING(SS_TAP(X_BSPC) SS_TAP(X_BSPC) "myname@email.com");
        //        return false;
        //}
    }


    if (!process_sequential_combo_keys(keycode, record)) return false;

    return true;
}

void housekeeping_task_user(void) {
    // 5秒間アイドルでIME OFF (英数キー送信)
    static bool idle_ime_sent = false;

    if (last_input_activity_elapsed() > 10000) {
        if (!idle_ime_sent) {
            tap_code(KC_LNG2);  // 英数キー (IME OFF)
            idle_ime_sent = true;
        }
    } else {
        idle_ime_sent = false;
    }

    recent_keys_housekeeping();
    sequential_combo_housekeeping();
}

// タッピングターム: キーを押してからホールドと判定されるまでの時間（ミリ秒）
// 値が小さいほど素早くホールド判定される。タップ入力には素早い指離しが必要になる
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case S_SLSH:
        case S_Z:
            return 180;
        case C_A:
//             return 170;
            return 180;
        case C_ENT:
            return 180;
        default:
            return 200;
    }
}

// クイックタップターム: 前回のタップから指定時間内に再度押した場合、自動的にタップとして扱う
// 0に設定すると、連打時でも常にホールド判定が行われる（リピート入力向け）
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case A_BSPC:
        case L1_BSPC:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

// パーミッシブホールド: true の場合、タッピングターム内でも
// 「このキーを押す→別キーを押して離す→このキーを離す」でホールド扱いになる
// false の場合、タッピングターム経過を待ってからホールド判定する
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case A_SPC:
        case G_TAB:
        case A_BSPC:
        case L1_BSPC:
//         case S_SLSH:
            return false;
        default:
           return PERMISSIVE_HOLD;
    }
}

// ホールドオン他キー押下: true の場合、このキーを押している間に
// 別のキーが押されると即座にホールドとして扱われる（タッピングターム無視）
// レイヤーキーなど、素早い修飾が必要な場合に有効
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case A_SPC:
        case G_TAB:
        case A_BSPC:
        case L1_BSPC:
//         case S_SLSH:
            return true;
        default:
            return HOLD_ON_OTHER_KEY_PRESS;
    }
}

// タッピングフォースホールド: true の場合、タップ直後に同じキーを押し続けても
// ホールド動作になる。false の場合、タップのオートリピートになる
// BSPCなど連打したいキーは false が便利
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case A_SPC:
        case G_TAB:
        case A_BSPC:
        case L1_BSPC:
//         case S_SLSH:
            return false;
        default:
            return TAPPING_FORCE_HOLD;
    }
}

