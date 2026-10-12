/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b28d0_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y -= 0x3000;
    }
    if ((u8)func_80130184(obj) != 0) {
        if ((u8)obj->step_flags != 2) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
    }
}
