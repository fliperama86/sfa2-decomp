/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001ac50_slot28(Object *o) {
    if (o->field_48 != 0) {
        o->field_05++;
        o->field_48 = 0;
        func_80130768(o, 2, (SequenceStep **)o->box_tables);
    }
}

void func_8001ac90_slot28(Object *o) {
    if (o->field_48 != 0) {
        o->field_05++;
        o->field_48 = 0;
        o->field_46 = 0;
        func_80130768(o, 3, (SequenceStep **)o->box_tables);
    }
}

void func_8001acd4_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    int t = obj->field_47 + 1;
    int d = 0x10000;
    obj->field_47 = t & 3;
    if (t & 2) {
        d = -0x10000;
    }
    obj->field_14 = obj->field_14 + d;
}
