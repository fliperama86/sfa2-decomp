/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079140_slot00[];
void func_80075e50_slot00(Object *obj);
void func_80075e90_slot00(Object *obj);
void func_80075fc8_slot00(Object *obj);
void func_800760a8_slot00(Object *obj);

void func_80075e10_slot00(Object *obj) {
    if (obj->input_dir_0 == 0) {
        func_80075e50_slot00(obj);
    } else {
        func_800760a8_slot00(obj);
    }
}

void func_80075e50_slot00(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_80075e90_slot00(obj);
    } else {
        func_80075fc8_slot00(obj);
    }
}

void func_80075e90_slot00(Object *obj) {
    data_80079140_slot00[obj->state_3](obj);
}

void func_80075ed0_slot00(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x11, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}
