/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern FrameRecord *data_1f8000a8;
extern FrameRecord *data_1f800158;
void func_80077050_slot00(Object *obj);
void func_800770b8_slot00(Object *obj);

void func_800766a0_slot00(Object *obj) {
    Slot00Obj *o = (Slot00Obj *)obj;
    int speeds[4] = { 0x38000, 0x40000, 0x50000, 0x48000 };
    int a, b;
    obj->field_09 = 0;
    obj->field_04 = obj->field_04 + 1;
    ref_other.p = obj->field_3c;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    a = obj->field_ac;
    o->field_b0 = 5;
    o->field_b2 = 0xb;
    b = 3;
    obj->field_50 = 0;
    obj->field_46 = 0;
    obj->field_03 = 0;
    o->field_b1 = 0;
    o->field_b3 = 0;
    if (obj->field_ad != 0) {
        a = 6;
        b = 1;
    }
    obj->field_a0 = b;
    if (obj->field_0b != 0) {
        obj->field_4c = speeds[a >> 1];
    } else {
        obj->field_4c = -speeds[a >> 1];
    }
    if (ref_other.p->side == 0) {
        obj->frames = data_1f8000a8;
    } else {
        obj->frames = data_1f800158;
    }
    func_80138070(obj, 2);
    func_80077050_slot00(obj);
    func_800770b8_slot00(obj);
}
