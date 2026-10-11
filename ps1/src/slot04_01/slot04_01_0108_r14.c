/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2720_slot04_01(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b2778_slot04_01(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
    }
    update_sequence(obj);
}

void func_801b2810_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
