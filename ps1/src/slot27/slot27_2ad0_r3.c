/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013cc4_slot27(Object *obj);

void func_80012d78_slot27(Object *obj) {
    ref_first.p = (Object *)obj->field_54;
    if (ref_other.p->field_d8 == 0) {
        ref_first.p = (Object *)obj->field_58;
    }
    if ((obj->field_07 & 1) == 0) {
        int *x = (int *)data_8019045c;
        *x = obj->field_4c;
        ref_first.p->field_4c += *x;
        *x = obj->field_50;
        if (ref_first.p->field_4c == *x) {
            obj->field_07 |= 1;
        }
    }
    if (obj->field_22 & 0x8000) {
        obj->field_07 |= 2;
    }
    if (obj->field_07 == 3) {
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_05 = obj->field_05 + 1;
    }
    func_80013cc4_slot27(obj);
}
