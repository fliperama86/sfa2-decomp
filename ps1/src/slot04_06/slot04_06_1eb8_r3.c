/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);

void func_801b2168_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->mode != 0) {
        o->state_3++;
        obj->mode = 0;
    }
    update_sequence(o);
}

void func_801b21a8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->status_flags & 0x80) {
        o->other->other_state_code = 5;
        func_801312b8(o);
    } else {
        if (obj->mode != 0) {
            obj->mode = 0;
            o->active_flag_4 = 0;
        }
        if (o->vel_x >= 0) {
            func_801b4818_slot04_06(o);
        }
        func_80142adc(o);
        update_sequence(o);
    }
}
