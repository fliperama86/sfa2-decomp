/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void bob_pos_y_by_vel_y_and_advance_sequence_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->pos_y = obj->pos_y - o->vel_y;
    o->vel_y = o->vel_y + o->accel_y;
    if (obj->timer_reload != 0) {
        if (o->vel_y >= 0) {
            o->vel_y = 0x8000;
            obj->timer_reload = 0;
            o->accel_y = -0x400;
        }
    } else if (o->vel_y < 0) {
        o->vel_y = -0x8000;
        o->accel_y = 0x400;
        obj->timer_reload = 0xff;
    }
    advance_sequence_checked(o);
    o->field_01 = 1;
}
