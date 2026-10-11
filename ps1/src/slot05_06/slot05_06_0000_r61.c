/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd560_slot05_06[];
extern ObjectFn data_801dd56c_slot05_06[];

void func_801cd3ac_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cd410_slot05_06(Object *obj) {
    data_801dd560_slot05_06[obj->field_12a >> 1](obj);
}

void func_801cd454_slot05_06(Object *obj) {
    data_801dd56c_slot05_06[obj->state_3](obj);
}

void func_801cd494_slot05_06(Object *obj) {
    obj->state_3++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
