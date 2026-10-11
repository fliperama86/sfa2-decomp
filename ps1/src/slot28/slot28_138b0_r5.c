/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80023f30_slot28(Object *obj) {
    Object *o = obj;
    Slot28Obj *s = (Slot28Obj *)o;
    if (s->field_47 == 0) {
        s->field_10 = s->field_10 + o->vel_x;
        o->vel_x = o->vel_x + o->accel_x;
        if (o->pos_x < 0xdb) {
            o->pos_x = 0xda;
            o->state_1 = o->state_1 + 1;
        }
    } else {
        s->field_47--;
    }
}
