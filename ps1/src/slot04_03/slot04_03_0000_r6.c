/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b079c_slot04_03(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->field_70 > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->field_45 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        update_sequence(obj);
        func_80130ec0(obj);
        func_801307e0(obj, 0x1e);
    }
}

void func_801b084c_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->field_249 = 8;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
