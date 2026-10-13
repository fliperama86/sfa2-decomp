/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_y_offset_and_spawn_at_limit_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Object *n;
    if (screen_state->substep_index >= 3 && game_state.mode != 3) {
        int sum = *(u16 *)&o->pos_y + obj->pos_y_offset;
        int y;
        *(u16 *)&o->pos_y = sum;
        y = (s16)sum;
        if ((u32)y >= 0x1e1) {
            obj->field_01 = 0;
            obj->field_05 = obj->field_05 + 1;
            n = (Object *)pop_block_from_pool();
            if (n != 0) {
                n->active_count = 1;
                n->field_02 = 0xaa;
                n->table_index = 0x80;
            }
        }
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void move_y_offset_until_limit_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    if (screen_state->substep_index >= 3 && game_state.mode != 3) {
        int sum = *(u16 *)&o->pos_y + obj->pos_y_offset;
        int y;
        *(u16 *)&o->pos_y = sum;
        y = (s16)sum;
        if ((u32)y >= 0x201) {
            obj->field_05 = obj->field_05 + 1;
        }
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void func_80014730_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->state_1 = obj->state_1 + 1;
}
