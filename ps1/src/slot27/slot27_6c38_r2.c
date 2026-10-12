/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80029064_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void func_80016d90_slot27(Object *obj) {
    advance_sequence_checked(obj);
}

void func_80016db0_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->state_0 = obj->state_0 + 1;
}

void func_80016dc4_slot27(Object *obj) {
    func_8011f38c(obj);
}

void func_80016de4_slot27(Object *obj) {
    *(int *)data_8019045c = -0x27;
    if (game_state.selection != 0) {
        *(int *)data_8019045c = 7;
    }
    *(u16 *)&obj->pos_x += *(u16 *)data_8019045c;
    *(u16 *)&obj->pos_y -= 8;
}

void func_80016e2c_slot27(Object *obj) {
    ref_other.p = obj->parent;
    data_80029064_slot27[obj->state_0](obj);
}

void func_80016e7c_slot27(Object *obj) {
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
