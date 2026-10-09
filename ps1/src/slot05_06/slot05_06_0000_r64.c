/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd594_slot05_06[];
extern ObjectFn data_801dd5a0_slot05_06[];
u8 func_80149b80(Object *obj);
extern ObjectFn data_801dd5a8_slot05_06[];

void func_801cd838_slot05_06(Object *obj) {
    data_801dd594_slot05_06[obj->field_12a >> 1](obj);
}

void func_801cd87c_slot05_06(Object *obj) {
    data_801dd5a0_slot05_06[obj->field_07](obj);
}

void func_801cd8bc_slot05_06(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801cd8f4_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801cd958_slot05_06(Object *obj) {
    data_801dd5a8_slot05_06[obj->field_07](obj);
}
