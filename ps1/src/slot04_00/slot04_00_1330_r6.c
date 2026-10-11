/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bfe40_slot04_00[];

void func_801b1998_slot04_00(Object *o) {
    u8 t = 1;
    s32 v;

    o->field_17b = t;
    o->state_3++;
    func_80141f28(o, 6);
    func_80138ae8(&game_state, o);
    o->pos_y_frac = 0;
    o->vel_y = 0x40000;
    o->field_45 = t;
    if (o->field_49 == 0) {
        o->accel_y = -0x6000;
    } else {
        o->accel_y = 0xfffe8000;
    }
    v = data_801bfe40_slot04_00[o->field_12a >> 1];
    if (o->field_0b == 0) {
        v = -v;
    }
    o->vel_x = v;
    func_801307e0(o, 0x1e);
}

void func_801b1a54_slot04_00(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int a = 0x20;

    *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    *(s32 *)&o->pos_y_frac = *(s32 *)&o->pos_y_frac - o->vel_y;
    o->vel_y = o->vel_y + o->accel_y;
    if (o->vel_y >= 0) {
        update_sequence(o);
    } else {
        o->state_3++;
        func_80120554(o, o->side, 0x320);
        obj->field_1a4 = o->field_12a >> 1;
        if (o->field_49 != 0) {
            a = 0x42;
        }
        func_801307e0(o, (o->field_12a >> 1) + a);
    }
}
