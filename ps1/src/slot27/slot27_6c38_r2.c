/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80029064_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void call_advance_sequence_checked_2_slot27(Object *obj) {
    advance_sequence_checked(obj);
}

void hide_object_and_advance_state_0_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->state_0 = obj->state_0 + 1;
}

void load_velocities_from_table_2_slot27(Object *obj) {
    func_8011f38c(obj);
}

void offset_position_by_selection_slot27(Object *obj) {
    *(int *)data_8019045c = -0x27;
    if (game_state.selection != 0) {
        *(int *)data_8019045c = 7;
    }
    *(u16 *)&obj->pos_x += *(u16 *)data_8019045c;
    *(u16 *)&obj->pos_y -= 8;
}

void call_handler_by_state_0_with_parent_slot27(Object *obj) {
    ref_other.p = obj->parent;
    data_80029064_slot27[obj->state_0](obj);
}

void init_object_images_copy_other_ot_depth_slot27(Object *obj) {
    obj->field_90 = (void *)0x80038000;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->spawn_x = 0;
    obj->image_row = 0;
    obj->variant_index = 0xff;
    obj->state_0++;
    obj->ot_depth = ref_other.p->ot_depth;
    obj->field_01 = 0;
}
