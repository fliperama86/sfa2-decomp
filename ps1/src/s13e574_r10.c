/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_config_table_handler(void) {
    table_8017ac44[game_state.config->handler_index]();
}

void update_sequence_by_pressed_bytes(Object *object) {
    if (object->mode_flag_2 != 0) {
        update_sequence_by_random_bit_twice(object);
    } else {
        if (object->pressed_bits & 0xff00) update_sequence(object);
        if (*(u8 *)&object->pressed_bits != 0) update_sequence(object);
    }
}

void update_sequence_by_random_bit_twice(Object *object) {
    if (test_random_bit_in_table(object, table_8017d618)) update_sequence(object);
    if (test_random_bit_in_table(object, table_8017d618)) update_sequence(object);
}
