/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f40_slot04_01(Object *obj) {
    obj->field_07++;
    obj->field_14 = 0;
    obj->field_17b = 1;
    obj->field_29c = 2;
    obj->field_45 = 1;
    if (obj->field_0b == 0) {
        *(s32 *)&obj->field_4c = -0x40000;
    } else {
        *(s32 *)&obj->field_4c = 0x40000;
    }
    func_801307e0(obj, 0x48);
}

void func_801b2f9c_slot04_01(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        obj->field_07++;
        if (obj->field_0b == 0) {
            obj->pos_x -= 0x14;
        } else {
            obj->pos_x += 0x14;
        }
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x317);
        func_80131d68(obj);
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
}

void func_801b3030_slot04_01(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_50 = 0x30000;
        obj->field_58 = -0x6000;
        obj->field_07++;
        if (obj->field_0b == 0) {
            obj->field_4c = 0x38000;
        } else {
            obj->field_4c = -0x38000;
        }
    }
}
