/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

void func_8013d65c(Object *object, u8 index, u8 arg) {
    u16 buttons;
    s16 dir;

    object->slots[index].timer--;
    if (object->slots[index].timer == 0) {
        func_8013f2a8(object, index, arg);
        return;
    }
    if (data_801a6984[0] == 0) {
        if (object->side == 0) {
            buttons = data_801a6966 & 0xf000;
        } else {
            buttons = data_801a6972 & 0xf000;
        }
    }
    if (game_state.versus_flag != 0 && data_801a6984[0] != 0 &&
        game_state.mode == object->field_02 + 1) {
        buttons = object->input_bits & 0xf000;
    }
    if (buttons != 0) {
        func_8013dce0(object, index, arg);
        if (buttons == data_80188f4c) {
            dir = 1;
            goto apply;
        }
        if (buttons == data_80188f50) {
            dir = -1;
            goto apply;
        }
    }
    func_8013f2c8(object);
    return;
apply:
    object->slots[index].stage++;
    object->slots[index].repeat_count--;
    object->slots[index].direction = dir;
    if (dir == -1) {
        object->slots[index].entry_index--;
    } else {
        object->slots[index].entry_index++;
    }
    object->slots[index].entry_index &= 3;
    set_slot_random_timer(object, index, arg);
}
