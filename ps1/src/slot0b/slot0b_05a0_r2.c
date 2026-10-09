/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e08d8_slot0b(ModObj *obj);
void func_801e0578_slot0b(Object *obj, u8 a);

void func_801e06d0_slot0b(ModObj *obj) {
    int t;
    obj->field_24 += 0x10;
    obj->field_20 += 1;
    obj->field_22 += 1;
    if (data_80190468.p->field_ab != 1) {
        t = obj->field_46;
        t--;
        obj->field_46 = t;
        if ((s16)t >= 0) {
            return;
        }
    }
    obj->field_24 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_05 += 1;
    func_801e08d8_slot0b(obj);
}

void func_801e0760_slot0b(ModObj *obj) {
    if (data_80190468.p->field_ab == 2) {
        obj->field_46 = 0xf;
        obj->field_05 += 1;
        ref_other.p = obj->field_3c;
        func_801e0578_slot0b((Object *)obj, func_80125b18(ref_other.p->kind));
        if (obj->field_30 != 0) {
            obj->field_30->field_04 = 2;
        }
    }
}
