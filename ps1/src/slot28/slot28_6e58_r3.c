/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800306c4_slot28[])(Object *);

void func_80017018_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->pos_y -= obj->delta_y;
    obj->delta_y += obj->field_58;
    if (obj->timer_reload != 0) {
        if (obj->delta_y >= 0) {
            obj->delta_y = 0x4000;
            obj->timer_reload = 0;
            obj->field_58 = -0x200;
        }
    } else if (obj->delta_y < 0) {
        obj->delta_y = -0x4000;
        obj->field_58 = 0x200;
        obj->timer_reload = 0xff;
    }
    obj->field_01 = 1;
}

void func_80017090_slot28(Object *obj) {
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}

void func_800170c0_slot28(Object *obj) {
    data_800306c4_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}

void func_8001711c_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1 += 1;
    }
}
