/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);

void func_801b2e68_slot04_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        func_80146998(obj);
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b2ebc_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->vel_x = 0xc0000;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->accel_x = 0xffff0000;
        obj->state_3++;
        func_801307e0(obj, 0x3a);
    }
}

void func_801b2f24_slot04_06(Object *obj) {
    u8 t = obj->step_flags;

    if (t != 0) {
        if (t != 2 && obj->vel_x >= 0) {
            func_801b4818_slot04_06(obj);
            update_sequence(obj);
        } else {
            obj->accel_x = -0x6000;
            obj->state_3++;
            update_sequence(obj);
        }
    } else {
        update_sequence(obj);
    }
}

void func_801b2fa8_slot04_06(Object *obj) {
    if (obj->vel_x >= 0) {
        func_801b4818_slot04_06(obj);
        update_sequence(obj);
    } else {
        obj->state_3++;
        update_sequence(obj);
    }
}
