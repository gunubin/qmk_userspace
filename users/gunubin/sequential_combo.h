// sequential_combo.h
#ifndef SEQUENTIAL_COMBO_H
#define SEQUENTIAL_COMBO_H

#include "quantum.h"

#define SEQUENTIAL_COMBO_TIMEOUT_MS 200

typedef struct {
    uint16_t first_key;
    uint16_t second_key;
    uint16_t output_key;
} sequential_combo_entry_t;

void init_sequential_combos(const sequential_combo_entry_t *entries, size_t count);
bool process_sequential_combo_keys(uint16_t keycode, keyrecord_t *record);
void sequential_combo_housekeeping(void);

#endif // SEQUENTIAL_COMBO_H
