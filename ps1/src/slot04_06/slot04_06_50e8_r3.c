/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b55cc_slot04_06(Object *obj) {
    obj->state_3++;
    if (obj->hit_flag_a != 0) {
        if ((u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}
