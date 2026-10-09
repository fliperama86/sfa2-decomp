/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);
extern ObjectFn data_801dd574_slot05_06[];
u8 func_8013f8c4(Object *obj, int a, int b);
void func_80130dc0(Object *obj);

void func_801cd524_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cd588_slot05_06(Object *obj) {
    data_801dd574_slot05_06[obj->field_07](obj);
}

void func_801cd5c8_slot05_06(Object *obj) {
    obj->field_07++;
    if (obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x18, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801cd640_slot05_06(Object *obj) {
    int d = 0x18;

    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    func_80130efc(obj);
}
