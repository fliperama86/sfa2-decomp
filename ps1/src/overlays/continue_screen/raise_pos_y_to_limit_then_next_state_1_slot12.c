/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void raise_pos_y_to_limit_then_next_state_1_slot12(Object *obj) {
    s16 y;

    if (obj->variant_index == 2 && obj->table_index == 0) {
        obj->state_1 = obj->state_1 + 1;
        return;
    }
    y = obj->pos_y;
    if (!(((Slot12Obj *)obj)->limit_y < y)) {
        obj->state_1 = obj->state_1 + 1;
        if (obj->table_index == 4) {
            game_state.field_2bc = 8;
        }
    } else {
        obj->pos_y = y - 4;
    }
}

void advance_state_0_clear_state_1_if_field_ten_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->state_0 = obj->state_0 + 1;
        obj->state_1 = 0;
    }
}

void release_object_to_pool_table_10_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

extern void (*data_80028a04_slot12[])(Object *);

void call_handler_by_state_0_7_slot12(Object *obj) {
    ref_game_state.p = (Object *)&game_state;
    data_80028a04_slot12[obj->state_0](obj);
}
