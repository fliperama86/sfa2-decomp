/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077908_slot2b(Object *obj);

void func_800777c8_slot2b(Object *obj) {
    Slot2bObj *o = (Slot2bObj *)obj;
    o->field_10 += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (o->field_3a == 0) {
        obj->state_3++;
        obj->pos_x_frac = 0;
    }
    update_sequence(obj);
}

void func_80077828_slot2b(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_80131468(obj);
    }
}
