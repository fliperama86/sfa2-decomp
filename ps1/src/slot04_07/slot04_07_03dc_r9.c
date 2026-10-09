/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c1ea0_slot04_07[];
extern u8 data_801ad398;
extern ObjectFn data_801c1ec8_slot04_07[];

void func_801b1960_slot04_07(Object *obj) {
    data_801ad398 = data_801c1ea0_slot04_07[obj->field_15a](obj);
}

int func_801b19a8_slot04_07(Object *obj) {
    return 1;
}

int func_801b19b0_slot04_07(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b19c4_slot04_07(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b19d0_slot04_07(Object *obj) {
    data_801c1ec8_slot04_07[obj->field_15a](obj);
}
