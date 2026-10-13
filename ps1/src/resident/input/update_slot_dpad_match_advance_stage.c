/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 pad_1_held;

void update_slot_dpad_match_advance_stage(Object *object, u8 index, u8 arg) {
    u16 buttons;

    object->slots[index].timer--;
    if (object->slots[index].timer == 0) {
        reset_slot_stage_clear_result(object, index, arg);
        return;
    }
    if (input_block[0] == 0) {
        if (object->side == 0) {
            buttons = pad_1_held & 0xf000;
        } else {
            buttons = pad_2_held & 0xf000;
        }
    }
    if (game_state.versus_flag != 0 && input_block[0] != 0 &&
        game_state.mode == object->field_02 + 1) {
        buttons = object->input_bits & 0xf000;
    }
    if (buttons == 0) {
        clear_global_flag_ignoring_object(object);
    }
    set_next_order_value_by_direction(object, index, arg);
    if (buttons == data_80188f4c) {
        goto apply;
    }
    clear_global_flag_ignoring_object(object);
    return;
apply:
    object->slots[index].stage++;
    set_slot_random_timer(object, index, arg);
}
