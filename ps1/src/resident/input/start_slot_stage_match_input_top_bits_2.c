/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void start_slot_stage_match_input_top_bits_2(Object *object, u8 index, u8 arg) {
    u16 entry;
    u8 step;

    object->slots[index].entry_index = 0;
    entry = slot_input_table[arg * 7];
    if ((entry & 0xf000) != (object->input_word & 0xf000)) {
        reset_slot_stage_clear_result(object, index, arg);
    } else {
        step = object->slots[index].stage;
        object->slots[index].stage = step + 1;
        if (!(entry & 1)) {
            set_slot_random_timer_and_advance(object, index, arg);
        } else {
            object->slots[index].stage = step + 2;
            set_slot_random_timer_and_advance(object, index, arg);
            check_slot_input_by_entry_index(object, index, arg);
        }
    }
}
