/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079a08_slot2b[];
extern Object *data_8007ef28_slot2b;
extern s16 data_8007ef2c_slot2b;

void func_8007810c_slot2b(Object *o) {
    Object *p = o->field_3c;
    data_8007ef28_slot2b = p;
    data_8007ef2c_slot2b = ((Slot2bObj *)p)->field_70;
    data_80079a08_slot2b[o->field_04](o);
}
