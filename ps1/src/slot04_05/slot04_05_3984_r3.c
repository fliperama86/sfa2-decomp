/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013f8c4(Object *obj, int a, int b);

void func_801b3dec_slot04_05(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && func_8013f8c4(obj, -0x17, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else if (obj->field_12a == 2) {
        if (obj->field_219 != 0) {
            obj->field_159 = 1;
            obj->field_07 = 2;
            func_80141f28(obj, 1);
            *(s32 *)&obj->field_4c = 0x68000;
            obj->field_54 = -0x8000;
            func_801307e0(obj, 0x34);
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
