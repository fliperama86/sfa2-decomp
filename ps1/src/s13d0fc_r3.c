/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void tick_slot_timer_and_check_input(Object *object, u8 index, u8 arg) {
    u16 entry;

    object->slots[index].timer--;
    if (object->slots[index].timer == 0) {
        reset_slot_stage_clear_result(object, index, arg);
    } else {
        entry = table_8017a8cc[arg * 7 + object->slots[index].entry_index];
        if ((entry & 0xf000) != (object->input_word & 0xf000)) {
            func_8013f2c8(object);
        } else {
            if (entry & 1) {
                object->slots[index].stage++;
            }
            set_slot_random_timer_and_advance(object, index, arg);
        }
    }
}

void count_down_slot_timer_check_button(Object *object, u8 index, u8 arg) {
    if (object->field_7e == 0) {
        object->slots[index].timer--;
        if (object->slots[index].timer == 0) {
            reset_slot_stage_clear_result(object, index, arg);
        }
    }
    if ((object->pressed_bits | object->released_bits) & 0x100) {
        reset_slot_stage_set_result(object, index, arg);
    } else {
        func_8013f2c8(object);
    }
}

u8 func_8013d4cc(Object *object, u8 index, u8 arg) {
    table_8017abc4[object->slots[index].stage](object, index, arg);
    return data_80188f44;
}
