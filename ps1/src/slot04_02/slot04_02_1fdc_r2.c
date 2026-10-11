/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b20d4_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u16 a;

    *(s32 *)&o->pos_x_frac += o->vel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
    if (o->vel_y < 0) {
        o->state_3++;
        func_80120554(o, o->side, 0x320);
        if (o->field_49 != 0) {
            a = 0x70;
        } else {
            a = 0x20;
        }
        if (o->field_12a != 0) {
            obj->field_1a4 = o->field_12a;
        } else {
            obj->field_1a4 = 1;
        }
        a += o->field_12a >> 1;
        func_801307e0(o, a);
    } else {
        update_sequence(o);
    }
}
