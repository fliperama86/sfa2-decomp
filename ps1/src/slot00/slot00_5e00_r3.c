/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079150_slot00[];
extern ObjectFn data_80079158_slot00[];
void func_800760e8_slot00(Object *obj);
void func_800761c8_slot00(Object *obj);

void func_800760a8_slot00(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second == 0) {
        func_800760e8_slot00(obj);
    } else {
        func_800761c8_slot00(obj);
    }
}

void func_800760e8_slot00(Object *obj) {
    data_80079150_slot00[obj->state_3](obj);
}

void func_80076128_slot00(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}

void func_80076160_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_800761c8_slot00(Object *obj) {
    data_80079158_slot00[obj->direction_index >> 1](obj);
}
