/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);

void func_801c8858_slot05_06(Object *obj) {
    int one;

    obj->field_4c = 0xa0000;
    obj->field_07 = obj->field_07 + 1;
    obj->field_54 = -0xc000;
    one = 1;
    obj->field_159 = one;
    func_80130dc0(obj);
    obj->field_278 = one;
}

void func_801c88b4_slot05_06(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_80131468(obj);
    } else {
        if ((s16)obj->field_3a == 3) {
            func_80120554(obj, obj->side, 0x324);
        }
        if ((s16)obj->field_3a == 2 && obj->field_4c >= 0) {
            func_801cc814_slot05_06(obj);
        } else {
            if (((*(u8 *)&obj->field_3a & 0x80) != 0 && (u8)func_80141618(obj) != 0) || (u8)func_801412a4(obj) != 0) {
                obj->field_07 = 0;
            }
        }
        func_80130efc(obj);
    }
}
