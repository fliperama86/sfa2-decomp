/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void tick_slot_timer_and_check_input_2(Object *object, u8 index, u8 arg) {
    u16 entry;

    object->slots[index].timer--;
    if (object->slots[index].timer == 0) {
        reset_slot_stage_clear_result(object, index, arg);
    } else {
        entry = table_8017a8cc[arg * 7 + object->slots[index].entry_index];
        if ((entry & 0xf000) != (object->input_word & 0xf000)) {
            clear_global_flag_ignoring_object(object);
        } else {
            if (entry & 1) {
                object->slots[index].stage++;
            }
            set_slot_random_timer_and_advance(object, index, arg);
        }
    }
}

