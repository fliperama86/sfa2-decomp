/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c28_slot04_0b(Object *obj);
void func_801b4db8_slot04_0b(Object *obj, int index);
void func_801b4e0c_slot04_0b(Object *obj, int dx, int dy);

void func_801b4acc_slot04_0b(Object *obj) {
    obj->field_0d = 0x1f;
    obj->field_05++;
    func_801b4e0c_slot04_0b(obj, 0xe, 0xe);
    func_801b4db8_slot04_0b(obj, 0x14);
    func_8011ffdc(obj);
}

void func_801b4b24_slot04_0b(Object *obj) {
    if (!(ref_other.p->field_3a & 1)) {
        obj->field_05++;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_10 = 0;
        obj->field_14 = 0;
        if (obj->field_0b != 0) {
            obj->field_5c = 0x340;
            obj->field_5e = 0x2c0;
            obj->field_4c = *(s32 *)&obj->field_10 + 0x40000;
            obj->field_50 = *(s32 *)&obj->field_10 + 0x110000;
        } else {
            *(s16 *)&obj->field_5c = -0x340;
            *(s16 *)&obj->field_5e = -0x2c0;
            obj->field_4c = *(s32 *)&obj->field_10 - 0x40000;
            obj->field_50 = *(s32 *)&obj->field_10 - 0x110000;
        }
        obj->field_70 = 0x500;
        obj->field_76 = 0x400;
        *(s16 *)&obj->field_26 = -0xa0;
        *(s16 *)&obj->field_46 = -0xa0;
        obj->field_54 = *(s32 *)&obj->field_14 - 0x340000;
        obj->field_58 = *(s32 *)&obj->field_14 - 0x4d0000;
        func_801b4c28_slot04_0b(obj);
    } else {
        func_8011ffdc(obj);
    }
}
