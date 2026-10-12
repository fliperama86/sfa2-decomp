/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b470c_slot04_07(Object *obj) {
    u8 one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0) {
        if (obj->hit_flag_a != 0 && (func_8013f8c4(obj, -0x12, 0x14) & 0xff) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->direction_index == 4) {
            obj->state_3 = 2;
            if (obj->attack_param_d != 0) {
                if (obj->facing_side == 0) {
                    obj->pos_x = obj->pos_x - 0x20;
                } else {
                    obj->pos_x = obj->pos_x + 0x20;
                }
            }
            func_801307e0(obj, 0x2f);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
