/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80076f44_slot00(Object *o) {
    SequenceStep **t;

    o->state_1++;
    if (o->field_3c->side == 0) {
        t = data_1f8000ac;
    } else {
        t = data_1f80015c;
    }
    func_80130768(o, 1, t);
}

void func_80076f9c_slot00(Object *o) {
    if ((s16)o->step_flags >= 0) {
        func_80131094(o);
    } else {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
    }
}

void func_80076fe4_slot00(Object *o) {
    ref_other.p = o->field_3c;
    ref_other.p->field_240--;
    if (ref_other.p->field_240 == 0) {
        ref_other.p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}
