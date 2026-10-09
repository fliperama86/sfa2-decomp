/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b599c_slot04_06(Object *obj) {
    obj->field_4c = 0xa0000;
    obj->field_54 = -0xc000;
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b59e8_slot04_06(Object *obj) {
    int d;

    if ((s16)obj->field_3a >= 0) {
        func_80131468(obj);
    } else {
        if ((s16)obj->field_3a == 3) {
            func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x324);
        }
        if ((s16)obj->field_3a == 2 && (s32)obj->field_4c >= 0) {
            d = obj->field_4c;
            if (obj->field_0b == 0) {
                d = -d;
            }
            *(s32 *)&obj->field_10 += d;
            obj->field_4c += obj->field_54;
            func_80130efc(obj);
        } else {
            if (((Slot04aObj *)obj)->field_3a != 0 && (func_80149b80(obj) & 0xff) != 0) {
                obj->field_07 = 0;
            }
            func_80130efc(obj);
        }
    }
}
