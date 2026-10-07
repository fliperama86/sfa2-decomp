/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a110_slot00[];

void func_800786f0_slot00(Object *obj) {
    Object *p = obj->field_3c;
    obj->field_a4 = obj->field_a4 - 1;
    if (obj->field_a4 & 0x80) {
        obj->field_a4 = 2;
        obj->field_0c = p->field_0c;
        obj->field_0d = p->field_0d;
        obj->field_a5 = obj->field_a5 ^ 1;
        if (obj->field_a5 != 0) {
            obj->field_0c = 0xff;
            obj->field_0d = obj->field_0d + 1;
        }
    }
    data_8007a110_slot00[obj->field_06](obj);
}

void func_80078790_slot00(Object *obj) {
    obj->field_46 = 0xa;
    obj->field_06 = obj->field_06 + 1;
    func_80131094(obj);
}

void func_800787c0_slot00(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_60 = 0x13;
        obj->field_61 = 0xf;
        obj->field_45 = 0x1d;
        obj->field_06 = obj->field_06 + 1;
    }
    func_80131094(obj);
}
