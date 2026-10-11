/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f98_slot04_05(Object *obj) {
    if (((Slot04aObj *)obj)->field_1a6 != 2) {
        func_801307e0(obj, 0x25);
        obj->state_0 = 1;
        obj->state_2 = 8;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 3;
    } else {
        func_801307e0(obj, 0x27);
        obj->state_0 = 1;
        obj->state_2 = 8;
        obj->vel_x = 0xc0000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x80000;
        obj->state_1 = 0;
        obj->state_3 = 1;
        obj->field_15a = 1;
        obj->accel_y = -0x6000;
        obj->field_45 = 1;
    }
    ((Slot04aObj *)obj)->field_1a6 = 0;
}

int func_801b3048_slot04_05(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    return obj->vel_x += obj->accel_x;
}
