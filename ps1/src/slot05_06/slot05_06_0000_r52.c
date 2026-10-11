/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd4f8_slot05_06[];

void func_801cc750_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->state_3++;
        if (obj->field_129 == 0) {
            func_80140770(obj, 0x26, 5, 0x11, 0, 0, 1);
        } else {
            func_80140770(obj, 0x29, 5, 1, 0, 0, 1);
        }
    }
    func_80130efc(obj);
}

void func_801cc7d4_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cc814_slot05_06(Object *obj) {
    int t = obj->vel_x;

    if (obj->field_0b == 0) {
        t = -t;
    }
    *(s32 *)&obj->pos_x_frac += t;
    obj->vel_x += obj->accel_x;
}

void func_801cc84c_slot05_06(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void func_801cc870_slot05_06(Object *obj, u16 *p) {
    if (obj->field_0b == 0) {
        *p = -*p;
    }
}

void func_801cc898_slot05_06(Object *obj) {
    data_801dd4f8_slot05_06[obj->state_2](obj);
}
