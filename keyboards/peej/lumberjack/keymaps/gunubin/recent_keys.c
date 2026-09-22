// ref https://getreuer.info/posts/keyboards/triggers/index.html
#include "recent_keys.h"
#include <string.h>

uint16_t recent[RECENT_SIZE] = {KC_NO};
static uint16_t deadline = 0;

void clear_recent_keys(void) {
    memset(recent, 0, sizeof(recent));  // Set all zeros (KC_NO).
}

bool update_recent_keys(uint16_t keycode, keyrecord_t* record) {
    if (!record->event.pressed) { return false; }

    if (((get_mods() | get_oneshot_mods()) & ~MOD_MASK_SHIFT) != 0) {
        clear_recent_keys();  // Avoid interfering with hotkeys.
        return false;
    }

    // Handle tap-hold keys.
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            if (record->tap.count == 0) { return false; }
            keycode &= 0xff;  // Get tapping keycode.
    }

    switch (keycode) {
        case KC_A ... KC_SLASH:  // These keys type letters, digits, symbols.
        case KC_LPRN:  // Add KC_LPRN to the valid key range.
        case KC_RPRN:
        case KC_LCBR:
        case KC_RCBR:
        case KC_PLUS:
        case KC_PIPE:
            break;

        case KC_LSFT:  // These keys don't type anything on their own.
        case KC_RSFT:
        case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
            return false;

        default:  // Avoid acting otherwise, particularly on navigation keys.
            clear_recent_keys();
            return false;
    }

    // Slide the buffer left by one element.
    memmove(recent, recent + 1, (RECENT_SIZE - 1) * sizeof(*recent));

    recent[RECENT_SIZE - 1] = keycode;
    deadline = record->event.time + TIMEOUT_MS;
    return true;
}

void recent_keys_housekeeping(void) {
    if (recent[RECENT_SIZE - 1] && timer_expired(timer_read(), deadline)) {
        clear_recent_keys();  // Timed out; clear the buffer.
    }
}
