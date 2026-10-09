/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4638_slot04_0b(Object *o);

void func_801b454c_slot04_0b(Object *o) {
    o->field_09 = 1;
    o->field_04++;
    o->field_0b = ref_other.p->field_0b;
    o->field_0c = ref_other.p->field_0c;
    o->field_0d = ref_other.p->field_0d;
    o->field_0e = ref_other.p->field_0e;
    o->field_0f = ref_other.p->field_0f;
    o->field_1c = ref_other.p->field_1c;
    o->field_26 = ref_other.p->field_26;
    if (o->field_03 != 0) {
        o->field_0d++;
    }
    func_801b4638_slot04_0b(o);
}
