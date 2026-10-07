/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007ed88_slot2b[];
extern Object *data_8007ef28_slot2b;
extern Object *data_8007ef30_slot2b;
void func_8011f14c(Slab172 *o);
void func_80078654_slot2b(Object *o);

void func_80078518_slot2b(Object *o) {
    Object *p = data_8007ef28_slot2b;

    if ((s32)o == p->field_14c) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_80078554_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;

    obj->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    obj->field_14 -= o->field_50;
    o->field_50 += o->field_58;
}

void func_80078598_slot2b(Object *o) {
    data_8007ef30_slot2b = o->field_3c;
    data_8007ed88_slot2b[o->field_04](o);
}

void func_800785e8_slot2b(Object *o) {
    Object *p;

    o->field_04++;
    p = data_8007ef30_slot2b;
    o->field_1c = p->field_1c;
    o->field_03 = p->kind;
    o->field_0e = data_8007ef30_slot2b->field_0e;
    o->field_48 = 0;
    o->field_46 = 0;
    func_80078654_slot2b(o);
}
