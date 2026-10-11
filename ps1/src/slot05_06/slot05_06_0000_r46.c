/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *object);
extern ObjectFn data_801dd4ac_slot05_06[];

void func_801cbe80_slot05_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        func_80146998(obj);
    }
    update_sequence(obj);
}

void func_801cbed0_slot05_06(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_3++;
        obj->vel_x = 0xa0000;
        obj->accel_x = -0xc000;
        func_801307e0(obj, 0x1a);
    } else {
        update_sequence(obj);
    }
}

void func_801cbf2c_slot05_06(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if (t == 3) {
            func_80120554(obj, obj->side, 0x324);
        }
        if ((s16)obj->step_flags == 2 && obj->vel_x >= 0) {
            func_801cc814_slot05_06(obj);
        }
        update_sequence(obj);
    }
}

void func_801cbfb4_slot05_06(Object *obj) {
    data_801dd4ac_slot05_06[obj->state_3](obj);
}
