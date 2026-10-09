/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142ba0(Object *object);
void func_801b3e44_slot04_0a(Object *obj);

void func_801b11bc_slot04_0a(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 9;
    obj->field_27b = 0x28;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x24;
    func_801307e0(obj, 0x1a);
}

int func_801b1218_slot04_0a(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        func_801b3e44_slot04_0a(obj);
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

int func_801b129c_slot04_0a(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 1;
        func_801b3e44_slot04_0a(obj);
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}
