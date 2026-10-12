/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 call_slot_stage_handler_in_first_table_4(Object *object, u8 index, u8 arg) {
    table_8017abec[object->slots[index].stage](object, index, arg);
    return data_80188f44;
}

void start_slot_stage_match_pressed_bits(Object *object, u8 index, u8 arg) {
    u16 entry;
    int mask;

    object->slots[index].entry_index = 0;
    entry = table_8017a8cc[arg * 7];
    mask = entry & 0xf0ff;
    if ((mask & object->pressed_bits) == 0) {
        reset_slot_stage_clear_result(object, index, arg);
    } else {
        object->slots[index].stage++;
        if (entry & 1) {
            reset_slot_stage_set_result(object, index, arg);
        } else {
            set_slot_random_timer_and_advance(object, index, arg);
        }
    }
}
