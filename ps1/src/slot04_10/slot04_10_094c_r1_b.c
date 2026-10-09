/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6998_slot04_10[];
extern ObjectFn data_801c69a0_slot04_10[];

void func_80130678(Object *object, int arg);
void func_80131468(Object *object);
void func_80131638(Object *object);

void func_801b10b8_slot04_10(Object *obj);
void func_801b1308_slot04_10(Object *obj);
void func_801b16c8_slot04_10(Object *obj);

void func_801b0fb0_slot04_10(Object *obj) {
    data_801c6998_slot04_10[obj->field_06](obj);
}

void func_801b0ff0_slot04_10(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    func_80130678(obj, 0x22);
    func_801204f4(obj, obj->side, 0xb);
}

void func_801b1038_slot04_10(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80130efc(obj);
}

void func_801b1078_slot04_10(Object *obj) {
    func_801312b8(obj);
}

void func_801b1098_slot04_10(Object *obj) {
    func_80131468(obj);
}

void func_801b10b8_slot04_10(Object *obj) {
    func_80131638(obj);
}

void func_801b10d8_slot04_10(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b16c8_slot04_10(obj);
    } else if (obj->field_129 != 0) {
        func_801b1308_slot04_10(obj);
    } else {
        data_801c69a0_slot04_10[obj->field_07](obj);
    }
}
