/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b42fc_slot04_03(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0) {
        if (obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 0;
        } else if (obj->direction_index != 2) {
            if (obj->attack_param_d != 0 && obj->direction_flag == 0) {
                obj->state_3 = 2;
                *(s16 *)&obj->countdown = -1;
                func_80141f28(obj, 2);
                func_801307e0(obj, 0x1c);
            } else {
                func_80130dc0(obj);
            }
        } else if (obj->attack_param_d != 0) {
            obj->state_3 = 3;
            obj->vel_y = 0xfffc0000;
            obj->accel_y = 0x6000;
            if (obj->facing_side != (obj->direction_flag ^ 1)) {
                obj->vel_x = 0xfffca000;
            } else {
                obj->vel_x = 0x36000;
            }
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x1d);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
