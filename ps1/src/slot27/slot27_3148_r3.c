/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013b74_slot27(Object *obj);
void func_80012fd4_slot27(Object *obj);

void func_800134e8_slot27(Object *obj) {
    s32 *t = (s32 *)data_8019045c;
    ref_first.p = (Object *)obj->field_54;
    if (obj->field_66 != 0) {
        ref_first.p = (Object *)obj->field_58;
    }
    *t = obj->field_4c;
    ref_first.p->field_4c = ref_first.p->field_4c + *t;
    if (ref_first.p->field_4c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
}

void func_80013570_slot27(Object *obj) {
    s32 *t = (s32 *)data_8019045c;
    s32 *u;
    s16 v;
    *t = (s16)obj->field_5e;
    obj->field_5c = obj->field_5c + *(u16 *)t;
    v = obj->field_5c;
    if (v == 0 || v == 0x10) {
        obj->field_06 = obj->field_06 + 2;
    }
    u = (s32 *)data_8019045c;
    *u = obj->field_62;
    if (obj->field_63 != *u) {
        obj->field_63 = *(u8 *)u;
        *u = *u + 8;
        func_80013b74_slot27(obj);
    }
    func_80012fd4_slot27(obj);
}
