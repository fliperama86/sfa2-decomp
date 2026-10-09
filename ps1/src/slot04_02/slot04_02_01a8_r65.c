/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7808_slot04_02[];
void func_801b7920_slot04_02(Object *obj, u8 a);
void func_801b7908_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b7384_slot04_02(Object *obj) {
    u8 a;

    obj->field_46 = 2;
    obj->field_06++;
    obj->field_0b ^= 1;
    func_801380f0(obj);
    a = data_801c7808_slot04_02[obj->field_ac];
    if (a != 0) {
        func_801b7920_slot04_02(obj, a);
    }
    func_80131094(obj);
}

void func_801b7400_slot04_02(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_00 = 1;
        func_801b7908_slot04_02(obj, 1, 0, 0, 0);
        obj->field_4c = -obj->field_4c;
        obj->field_50 = 0;
    }
    func_80131094(obj);
}
