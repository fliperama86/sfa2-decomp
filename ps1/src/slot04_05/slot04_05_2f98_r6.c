/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3734_slot04_05(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_0b = 0;
    if ((obj->field_c2 & 0x8000) != 0) {
        obj->field_0b = 1;
        obj->field_4c = -0x30000;
    } else {
        obj->field_4c = 0x30000;
    }
    obj->field_54 = 0;
    obj->field_50 = 0x40000;
    obj->field_58 = -0xe000;
    func_801307e0(obj, 0x19);
}

void func_801b37d0_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b3810_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a == 2) {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        func_80130efc(obj);
    }
}

void func_801b3864_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
    }
    if (*(u8 *)&obj->field_3a != 3) {
        func_80130efc(obj);
    }
}
