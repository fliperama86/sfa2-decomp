/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a11c_slot00[];
extern ObjectFn data_8007a124_slot00[];
void func_80078b50_slot00(Object *o);

void func_80078964_slot00(Object *obj) {
    data_8007a11c_slot00[obj->state_2](obj);
}

void func_800789a4_slot00(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_46 = 8;
        obj->state_2++;
    }
    func_80078b50_slot00(obj);
    advance_sequence_checked(obj);
}

void func_80078a04_slot00(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->state_0++;
    }
    if (obj->field_46 & 1) {
        obj->field_67 ^= 1;
    }
    advance_sequence_checked(obj);
}

void func_80078a74_slot00(Object *obj) {
    data_8007a124_slot00[obj->state_2](obj);
}
