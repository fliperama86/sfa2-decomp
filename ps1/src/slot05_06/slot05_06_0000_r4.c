/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd244_slot05_06[];
void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80142a14(Object *object);
extern ObjectFn data_801dd24c_slot05_06[];

void func_801c83e8_slot05_06(Object *obj) {
    data_801dd244_slot05_06[obj->field_07](obj);
}

void func_801c8428_slot05_06(Object *obj) {
    obj->field_07++;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x18, 0x16) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801c84b0_slot05_06(Object *obj) {
    func_80142a14(obj);
}

void func_801c84d0_slot05_06(Object *obj) {
    data_801dd24c_slot05_06[obj->field_07](obj);
}
