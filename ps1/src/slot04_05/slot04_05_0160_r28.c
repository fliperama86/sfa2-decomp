/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);
void func_801b4160_slot04_05(Object *obj);
void func_801b4240_slot04_05(Object *obj);
extern ObjectFn data_801c166c_slot04_05[];
void func_80130dc0(Object *obj);

void func_801b40b8_slot04_05(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4120_slot04_05(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 == 0) {
        func_801b4160_slot04_05(obj);
    } else {
        func_801b4240_slot04_05(obj);
    }
}

void func_801b4160_slot04_05(Object *obj) {
    data_801c166c_slot04_05[obj->field_07](obj);
}

void func_801b41a0_slot04_05(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b41d8_slot04_05(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
