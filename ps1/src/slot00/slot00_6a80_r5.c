/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000ac;
extern SequenceStep **data_1f80015c;

void func_80076f44_slot00(Object *o) {
    SequenceStep **t;

    o->field_05++;
    if (o->field_3c->side == 0) {
        t = data_1f8000ac;
    } else {
        t = data_1f80015c;
    }
    func_80130768(o, 1, t);
}

void func_80076f9c_slot00(Object *o) {
    if ((s16)o->field_3a >= 0) {
        func_80131094(o);
    } else {
        o->field_04 = 3;
        o->field_05 = 0;
        o->field_06 = 0;
        o->field_07 = 0;
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
