/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027cfc_slot27[];

void add_angle_step_and_advance_state_1_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    int v;
    if (data_8018f5a0->substep_index >= 3 && game_state.mode != 3) {
        obj->angle += obj->field_4c;
        v = (s16)obj->angle;
        if ((u32)v >= 0x381) {
            o->state_1++;
        }
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void hide_and_advance_state_1_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->state_1++;
}

void release_object_to_pool_table_5_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_state_1_handler_from_slot27_table_b_6_slot27(Object *obj) {
    data_80027cfc_slot27[obj->state_1](obj);
}

void start_spawn_x10_move_right_slot27(Object *obj) {
    obj->spawn_x = 0x10;
    obj->spawn_y = 0x1e0;
    obj->field_01 = 1;
    obj->vel_x = 8;
    obj->state_1++;
}
