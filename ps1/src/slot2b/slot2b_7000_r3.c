/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007981c_slot2b[];
extern ObjectFn data_80079828_slot2b[];
u8 func_8013f8c4(Object *obj, int a, int b);

void func_80077238_slot2b(Object *obj) {
    data_8007981c_slot2b[obj->field_12a >> 1](obj);
}

void func_8007727c_slot2b(Object *obj) {
    data_80079828_slot2b[obj->field_07](obj);
}

void func_800772bc_slot2b(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0xd, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            return;
        }
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->field_07 = 2;
            obj->field_159 = 1;
            obj->field_157 = 0;
            obj->field_45 = 1;
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x1c);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}
