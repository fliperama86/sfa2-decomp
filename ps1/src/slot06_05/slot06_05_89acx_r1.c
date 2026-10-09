/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801369b0(Cam *cam);

void func_801e89ac_slot06_05(Slot06Layer *o) {
    Slot06Layer *l2;
    s16 d;

    if (o->field_04 == 0) {
        func_801369b0((Cam *)o);
    } else if (o->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d -= d / 4;
        d += o->field_0a;
        d += o->field_36;
        o->field_22 = d;
        d = l2->field_26;
        d -= l2->field_0e;
        d -= d / 4;
        d += o->field_0e;
        d += o->field_3a;
        o->field_26 = d;
    }
}
