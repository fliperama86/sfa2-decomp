/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c0568_slot04_03[];
extern u8 data_801ad398;

void func_801b117c_slot04_03(Object *obj) {
    data_801ad398 = data_801c0568_slot04_03[obj->field_15a](obj);
}

int func_801b11c4_slot04_03(Object *obj) {
    return 1;
}

int func_801b11cc_slot04_03(Object *obj) {
    return obj->field_240 == 0;
}
