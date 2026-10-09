/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c139c_slot04_05[];
extern ObjectFn data_801c13c4_slot04_05[];
extern ObjectFn data_801c13ec_slot04_05[];

void func_801b1454_slot04_05(Object *obj) {
    data_801ad398 = data_801c139c_slot04_05[obj->field_15a](obj);
}

int func_801b149c_slot04_05(Object *obj) {
    return 1;
}

int func_801b14a4_slot04_05(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b14b8_slot04_05(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b14cc_slot04_05(Object *obj) {
    return obj->field_177 != 0;
}

int func_801b14d8_slot04_05(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

void func_801b14ec_slot04_05(Object *obj) {
    data_801c13c4_slot04_05[obj->field_15a](obj);
}

void func_801b152c_slot04_05(Object *obj) {
    data_801c13ec_slot04_05[obj->field_15a](obj);
}
