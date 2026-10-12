/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4280_slot04_05(Object *obj) {
    obj->facing_side = obj->saved_facing;
    obj->state_3++;
    if (obj->direction_index == 2) {
        if (obj->attack_param_d != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x28);
        } else {
            obj->active_flag_3 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b4310_slot04_05(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
