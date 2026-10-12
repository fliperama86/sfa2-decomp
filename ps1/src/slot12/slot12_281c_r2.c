/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80022e98_slot12[])(Object *);

void call_handler_by_state_1_slot12(Object *obj) {
    data_80022e98_slot12[obj->state_1](obj);
}

void update_y_motion_with_countdown_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int v;

    *(s32 *)&o->pos_y_frac += obj->field_50;
    obj->field_50 += obj->accel_y_step;
    v = obj->countdown - 1;
    obj->countdown = v;
    if ((s16)v < 0) {
        o->pos_y = 0;
        obj->tick_count++;
        if (obj->field_03 != 0) {
            o->pos_y = 0x70;
        }
    }
}

void noop_slot00_3_slot12(Object *obj) {
}

void func_800129ec_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
