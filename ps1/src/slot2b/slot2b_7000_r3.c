/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007981c_slot2b[];
extern ObjectFn data_80079828_slot2b[];

void func_80077238_slot2b(Object *obj) {
    data_8007981c_slot2b[obj->direction_index >> 1](obj);
}

void func_8007727c_slot2b(Object *obj) {
    data_80079828_slot2b[obj->state_3](obj);
}

void func_800772bc_slot2b(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if ((u8)func_8013f8c4(obj, -0xd, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->state_3 = 2;
            obj->active_flag_3 = 1;
            obj->field_157 = 0;
            obj->active_state = 1;
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x1c);
            return;
        }
    }
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}
