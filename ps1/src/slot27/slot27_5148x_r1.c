/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_800282e8_slot27[];

void func_80015148_slot27(Object *obj) {
    int *p = data_8019045c;
    int idx;
    *p = obj->field_62;
    if (obj->field_45 != *p) {
        obj->field_45 = *p;
        *p = *p * 2;
        obj->field_09 = *p + 3;
        *p = *p * 2;
        idx = obj->field_03 + *p;
        *p = idx;
        func_80130768(obj, idx, data_800282e8_slot27[(*(Config **)&data_80190458)->field_a6]);
    }
}
