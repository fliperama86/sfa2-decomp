/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_801c2090_slot04_07[];
extern int data_801c209c_slot04_07[];

void func_80142adc(Object *object);
void func_801b3874_slot04_07(Object *obj);

void func_801b36d4_slot04_07(Object *obj) {
    int a;
    int b;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    a = data_801c2090_slot04_07[obj->field_12a >> 1];
    obj->field_54 = -0x10000;
    obj->field_48 = obj->field_0b;
    obj->field_0b = obj->field_0b ^ 1;
    obj->field_4c = a;
    b = 0x60;
    if (obj->field_49 != 0) {
        b = 0x63;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + b);
}

void func_801b3784_slot04_07(Object *obj) {
    int a;

    func_80130efc(obj);
    func_801b3874_slot04_07(obj);
    if (((Slot04aObj *)obj)->field_3a == 0) {
        obj->field_07++;
        obj->field_0b = obj->field_0b ^ 1;
        a = data_801c209c_slot04_07[obj->field_12a >> 1];
        obj->field_54 = -0x10000;
        obj->field_4c = a;
    }
}

void func_801b3804_slot04_07(Object *obj) {
    func_801b3874_slot04_07(obj);
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
        if (((Slot04aObj *)obj)->field_3a != 0) {
            obj->field_17b = 0;
        }
    }
}
