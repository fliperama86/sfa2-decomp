/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c25dc_slot04_0b[];

void func_801b22ec_slot04_0b(Object *o) {
    func_80130efc(o);
    if (((s16)o->step_flags & 0xff00) == 0) {
        o->state_3++;
        func_801204f4(o, o->side, 0xf);
        func_801204f4(o, o->side, 9);
        o->field_165 = 0;
        o->field_27b = data_801c25dc_slot04_0b[o->field_12a >> 1];
        if (o->field_4b == 0) {
            o->other->field_6b = 0xa;
        }
    }
}

void func_801b2384_slot04_0b(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_80130efc(o);
    if (obj->field_3a == 3) {
        o->field_45 = 1;
        o->state_3++;
        if (o->field_0b == 0) {
            o->vel_x = 0xfff60000;
        } else {
            o->vel_x = 0xa0000;
        }
    }
    *(s32 *)&o->pos_x_frac += o->vel_x;
}

void func_801b23f8_slot04_0b(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u16 t = o->step_flags;

    *(s32 *)&o->pos_x_frac += o->vel_x;
    if ((u8)t == 4) {
        o->step_flags = t & 0xff00;
    }
    if (obj->field_3a == 0x16) {
        o->state_3++;
    }
    func_80130efc(o);
}
