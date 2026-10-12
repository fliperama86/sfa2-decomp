/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2758_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y = obj->accel_y - 0x2000;
    }
    if ((u8)func_80130184(obj) != 0) {
        if (*(u8 *)&obj->step_flags != 2) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->pos_y = obj->ground_y;
        obj->field_45 = 0;
        if (obj->field_0b != 0) {
            obj->vel_x = 0x70000;
        } else {
            obj->vel_x = -0x70000;
        }
        obj->vel_y = 0x30000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
    }
}
