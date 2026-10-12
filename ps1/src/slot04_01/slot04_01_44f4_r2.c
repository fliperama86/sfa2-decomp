/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_child_sequence_from_parent_side_slot04_01(Object *obj) {
    obj->spawn_flag = 1;
    obj->state_0++;
    if (obj->parent->side == 0) {
        set_sequence_from_table_5(obj, obj->variant_index, data_1f8000b4);
    } else {
        set_sequence_from_table_5(obj, obj->variant_index, data_1f800164);
    }
}

void func_801b46dc_slot04_01(Object *obj) {
    Object *p = obj->parent;
    int a;

    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0++;
    }
    a = -0x18;
    if (obj->variant_index != 7) {
        a = 0x14;
    }
    if (obj->facing_side == 0) {
        obj->pos_x = a + p->pos_x;
    } else {
        obj->pos_x = p->pos_x - a;
    }
    advance_sequence_checked(obj);
    if ((obj->table_index + (game_state.frame_counter + p->side)) & 1) {
        set_flag_and_push_if_mode_8(obj);
    } else {
        obj->field_01 = 0;
    }
}
