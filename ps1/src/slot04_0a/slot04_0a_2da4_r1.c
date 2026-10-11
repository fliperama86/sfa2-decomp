/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c0424_slot04_0a[][21];

void func_801b2da4_slot04_0a(Object *obj) {
    Object *o;
    int t;

    obj->state_3++;
    obj->field_67 = 0;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    o = obj->other;
    t = *(s32 *)&o->pos_x_frac - *(s32 *)&obj->pos_x_frac;
    obj->vel_y = 0x60000;
    obj->accel_x = 0;
    obj->accel_y = -0x5000;
    obj->vel_x = t >> 5;
    if (o->field_45 != 0 || o->field_157 != 0) {
        t = 0;
    } else {
        t = 1;
    }

    t = data_801c0424_slot04_0a[t][o->kind];
    t <<= 16;
    t += *(s32 *)&obj->pos_y_frac - *(s32 *)&o->pos_y_frac;
    if (t > 0x900000) {
        t = 0x900000;
    }
    t >>= 5;
    obj->vel_y = obj->vel_y + t;
    if (obj->field_49 != 0) {
        func_801307e0(obj, 0x55);
    } else {
        func_801307e0(obj, 0x54);
    }
}
