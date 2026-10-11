/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0c04_slot04_09(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->field_10 += o->vel_x;
    o->vel_x += o->accel_x;
    if (o->vel_x == 0) {
        o->state_3++;
        o->pos_x_frac = 0;
    }
    if ((game_state.field_1d & 3) == 0) {
        if (func_80148e84(o) & 0xff) {
            if (o->field_0b == 0) {
                ref_other.p->pos_x -= 0x30;
            } else {
                ref_other.p->pos_x += 0x30;
            }
        }
    }
    func_80130efc(o);
}

void func_801b0cd4_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}
