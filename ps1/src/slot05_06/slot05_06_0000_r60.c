/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cd298_slot05_06(Object *obj);
void func_801cd410_slot05_06(Object *obj);
extern ObjectFn data_801dd54c_slot05_06[];
extern ObjectFn data_801dd558_slot05_06[];

void func_801cd258_slot05_06(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801cd410_slot05_06(obj);
    } else {
        func_801cd298_slot05_06(obj);
    }
}

void func_801cd298_slot05_06(Object *obj) {
    data_801dd54c_slot05_06[obj->direction_index >> 1](obj);
}

void func_801cd2dc_slot05_06(Object *obj) {
    data_801dd558_slot05_06[obj->state_3](obj);
}

void func_801cd31c_slot05_06(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}
