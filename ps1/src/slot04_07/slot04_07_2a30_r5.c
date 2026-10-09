/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 *data_801c2060_slot04_07[];

void func_801b3120_slot04_07(Object *obj);

void func_801b3054_slot04_07(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_80131468(obj);
    }
}

void func_801b309c_slot04_07(Object *obj) {
    int a;

    obj->field_07++;
    obj->field_67 = 0;
    ((Slot04aObj *)obj)->field_1cc = 0;
    a = obj->field_12a >> 1;
    a = data_801c2060_slot04_07[a][(s16)obj->field_46 >> 1];
    if (a >= 0) {
        func_801307e0(obj, a);
    } else {
        func_801b3120_slot04_07(obj);
    }
}

void func_801b3120_slot04_07(Object *obj) {
    obj->field_07 = 7;
    obj->field_50 = 0;
    obj->field_58 = -0x5600;
    obj->field_4c = 0;
    obj->field_54 = 0;
    func_80130678(obj, 0x21);
}
