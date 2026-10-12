/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b45b4_slot04_03(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->field_45 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        func_801209c4(obj);
        update_sequence(obj);
        func_801307e0(obj, 0x1e);
    }
}

void func_801b465c_slot04_03(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->other->field_249 = 8;
        func_801312b8(obj);
    }
}
