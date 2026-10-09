/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1d44_slot04_04(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 0;
        obj->field_07++;
    } else {
        if (obj->field_4c < 0) {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
    }
    func_80130efc(obj);
}

void func_801b1de8_slot04_04(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xb);
        obj->field_4c = obj->field_50;
        obj->field_54 = obj->field_58;
    }
    func_80130efc(obj);
}

void func_801b1e50_slot04_04(Object *obj) {
    if (!((s16)obj->field_3a & 0x8000)) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_4c = 0;
        obj->field_54 = 0;
        obj->field_50 = 0;
        obj->field_58 = 0;
        func_801312b8(obj);
    }
}
