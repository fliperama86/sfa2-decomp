/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);

void func_801b1600_slot04_07(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 4;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    obj->field_157 = 0;
    obj->other->field_6b = 0x11;
    obj->field_6b = 0;
    obj->field_27b = 0x15;
    func_801307e0(obj, 0x1d);
}

int func_801b1668_slot04_07(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 2;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
    return r;
}

int func_801b16f4_slot04_07(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 5;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        ((Slot04aObj *)obj)->field_1cd = obj->field_4b;
        func_80142718(obj);
    }
    return r;
}
