/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_x_with_accel_after_delay_slot28(Object *obj) {
    Object *o = obj;
    Slot28Obj *s = (Slot28Obj *)o;
    if (s->timer_reload == 0) {
        s->pos_x = s->pos_x + o->vel_x;
        o->vel_x = o->vel_x + o->accel_x;
        if (o->pos_x < 0xdb) {
            o->pos_x = 0xda;
            o->state_1 = o->state_1 + 1;
        }
    } else {
        s->timer_reload--;
    }
}
