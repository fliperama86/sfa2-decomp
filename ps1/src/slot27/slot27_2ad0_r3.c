/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013cc4_slot27(Object *obj);

void func_80012d78_slot27(Object *obj) {
    ref_first.p = (Object *)((ObjectWords *)obj)->word_54;
    if (ref_other.p->field_d8 == 0) {
        ref_first.p = (Object *)((ObjectWords *)obj)->word_58;
    }
    if ((obj->state_3 & 1) == 0) {
        int *x = (int *)data_8019045c;
        *x = obj->vel_x;
        ref_first.p->vel_x += *x;
        *x = obj->vel_y;
        if (ref_first.p->vel_x == *x) {
            obj->state_3 |= 1;
        }
    }
    if (obj->y_scale & 0x8000) {
        obj->state_3 |= 2;
    }
    if (obj->state_3 == 3) {
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->state_1 = obj->state_1 + 1;
    }
    func_80013cc4_slot27(obj);
}
