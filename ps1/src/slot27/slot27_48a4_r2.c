/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027d0c_slot27[];
extern SequenceStep *data_800280bc_slot27[];

void show_object_and_release_on_step_flags_slot27(Object *obj) {
    obj->field_01 = 1;
    obj->spawn_x = 0x40;
    obj->spawn_y = 0x1e0;
    obj->image_row = 0;
    if (obj->step_flags & 1) {
        game_state.busy_flag = 0;
    }
    if ((s16)obj->step_flags & 0x8000) {
        reset_and_push_object_to_pool_table((Slab172 *)obj);
    }
}

void func_80014a34_slot27(Object *obj) {
    data_80027d0c_slot27[obj->state_1](obj);
}

void place_by_other_side_and_start_sequence_slot27(Object *obj) {
    Object *other = obj->other;
    if (((game_state.mode | game_state.side_mask) >> other->side) & 1) {
        obj->image_row = 0x1b;
        obj->pos_x = 0x1b0;
        obj->state_1++;
        if (other->side != 0) {
            obj->pos_x = 0x250;
        }
        set_sequence_from_table_5(obj, other->kind, data_800280bc_slot27);
        obj->sound_side = other->kind;
        func_8011ffdc(obj);
    }
}
