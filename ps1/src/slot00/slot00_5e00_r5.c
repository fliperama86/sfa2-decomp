/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);

void func_8007632c_slot00(Object *obj) {
    obj->field_07++;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    if (obj->field_219 != 0) {
        obj->field_07 = 2;
        obj->field_225 = 1;
        if (obj->field_246 == 0) {
            obj->field_225 = 0;
            func_80141f28(obj, 1);
        }
        func_801204f4(obj, obj->side, 9);
        func_801307e0(obj, 0x2b);
    } else {
        func_80130dc0(obj);
    }
}

void func_800763c4_slot00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
