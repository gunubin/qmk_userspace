#ifndef RECENT_KEYS_H
#define RECENT_KEYS_H

#include QMK_KEYBOARD_H

#define TIMEOUT_MS 200  // Timeout in milliseconds.
#define RECENT_SIZE 3  // Number of keys in `recent` buffer.

extern uint16_t recent[RECENT_SIZE];

void clear_recent_keys(void);
bool update_recent_keys(uint16_t keycode, keyrecord_t* record);
void recent_keys_housekeeping(void);

#endif // RECENT_KEYS_H

