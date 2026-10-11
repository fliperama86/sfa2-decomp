/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int index);

/* functions of other units of this module */

void func_801b0f14_slot04_0f(Object *o) {
    *(s32 *)&o->pos_y_frac = *(s32 *)&o->pos_y_frac + o->vel_y;
    *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    if (o->field_70 <= o->pos_y) {
        o->state_3++;
        o->pos_y = (u16)o->field_70;
        func_801209c4(o);
        o->field_45 = 0;
        o->field_159 = 0;
        o->field_46 = (o->field_46 & 0xff00) | 8;
        func_80130678(o, 0x10);
    } else {
        func_80130efc(o);
    }
}
