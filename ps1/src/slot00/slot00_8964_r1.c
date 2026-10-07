/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a11c_slot00[];
extern ObjectFn data_8007a124_slot00[];
void func_80078b50_slot00(Object *o);

void func_80078964_slot00(Object *obj) {
    data_8007a11c_slot00[obj->field_06](obj);
}

void func_800789a4_slot00(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_46 = 8;
        obj->field_06++;
    }
    func_80078b50_slot00(obj);
    func_80131094(obj);
}

void func_80078a04_slot00(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_04++;
    }
    if (obj->field_46 & 1) {
        obj->field_67 ^= 1;
    }
    func_80131094(obj);
}

void func_80078a74_slot00(Object *obj) {
    data_8007a124_slot00[obj->field_06](obj);
}
