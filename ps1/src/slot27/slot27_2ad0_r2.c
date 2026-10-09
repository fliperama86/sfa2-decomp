/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013b74_slot27(Object *obj);

void func_80012cd0_slot27(Object *obj) {
    int *x = (int *)data_8019045c;
    int *y = data_80190464;
    obj->field_07 = 0;
    obj->field_06 = obj->field_06 + 1;
    *x = -0x10;
    *y = -0x90;
    if (ref_other.p->side != 0) {
        *x = 0x10;
        *y = 0x90;
    }
    obj->field_4c = *x;
    obj->field_50 = *y;
    *x = 4;
    *x = ref_other.p->field_d8 + 4;
    func_80013b74_slot27(obj);
}
