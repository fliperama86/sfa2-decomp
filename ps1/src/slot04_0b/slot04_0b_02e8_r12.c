/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c24fc_slot04_0b[];
extern u8 data_801ad398;

void func_801b14ac_slot04_0b(Object *obj) {
    data_801ad398 = data_801c24fc_slot04_0b[obj->field_15a](obj);
}

int func_801b14f4_slot04_0b(Object *obj) {
    return obj->field_177 != 0;
}

int func_801b1500_slot04_0b(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}
