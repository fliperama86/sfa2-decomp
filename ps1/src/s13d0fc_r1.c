/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int is_input_masked_equal_to_one_of_two_patterns_b(Object *object) {
    int bits = (object->pressed_bits | object->released_bits) & 0x6a;

    if (bits != 0x68) {
        return bits == 2;
    }
    return 1;
}

u8 get_slot_result_via_first_stage_table(Object *object, u8 index, u8 arg) {
    return call_slot_stage_handler_in_first_table_3(object, index, arg);
}

u8 get_slot_result_via_first_stage_table_2(Object *object, u8 index, u8 arg) {
    return call_slot_stage_handler_in_first_table_4(object, index, arg);
}

u8 get_slot_result_via_first_stage_table_3(Object *object, u8 index, u8 arg) {
    return call_slot_stage_handler_in_first_table(object, index, arg);
}
