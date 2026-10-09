/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c41a8_slot04_04[];
extern ObjectFn data_801c41d4_slot04_04[];

int func_801b15b4_slot04_04(Object *obj) {
    return 1;
}

int func_801b15bc_slot04_04(Object *obj) {
    return obj->field_240 == 0;
}

int func_801b15c8_slot04_04(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b15dc_slot04_04(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b15f0_slot04_04(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b1604_slot04_04(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1610_slot04_04(Object *obj) {
    data_801c41a8_slot04_04[obj->field_15a](obj);
}

void func_801b1650_slot04_04(Object *obj) {
    data_801c41d4_slot04_04[obj->field_15a](obj);
}
