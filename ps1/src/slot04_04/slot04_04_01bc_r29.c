/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4374_slot04_04[];

void func_801b25dc_slot04_04(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_27b = data_801c4374_slot04_04[3];
        } else {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
            obj->field_27b = data_801c4374_slot04_04[obj->field_12a >> 1];
        }
        obj->field_165 = 0;
        func_801204f4(obj, obj->side, 5);
    }
}

void func_801b2688_slot04_04(Object *obj) {
    u16 t;

    if (obj->field_4c != 0) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c - obj->field_54;
        if (obj->field_4c < 0) {
            obj->field_4c = 0;
        }
    }
    t = obj->field_3a;
    if (t & 0xff) {
        obj->field_3a = t & 0xff00;
        obj->field_4c = obj->field_50;
        obj->field_54 = obj->field_58;
    }
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
