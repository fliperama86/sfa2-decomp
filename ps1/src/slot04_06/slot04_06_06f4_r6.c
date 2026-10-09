/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c52b8_slot04_06[];

int func_801b13b8_slot04_06(Object *obj) {
    return obj->field_177 != 0;
}

int func_801b13c4_slot04_06(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

void func_801b13d8_slot04_06(Object *obj) {
    data_801c52b8_slot04_06[obj->field_15a](obj);
}
