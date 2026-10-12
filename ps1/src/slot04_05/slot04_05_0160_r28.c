/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4160_slot04_05(Object *obj);
void func_801b4240_slot04_05(Object *obj);
extern ObjectFn data_801c166c_slot04_05[];

void func_801b40b8_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4120_slot04_05(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second == 0) {
        func_801b4160_slot04_05(obj);
    } else {
        func_801b4240_slot04_05(obj);
    }
}

void func_801b4160_slot04_05(Object *obj) {
    data_801c166c_slot04_05[obj->state_3](obj);
}

void func_801b41a0_slot04_05(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}

void func_801b41d8_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
