/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801bfd78_slot04_00[];
extern u8 data_801ad398;

void func_801b11c0_slot04_00(Object *obj) {
    data_801ad398 = data_801bfd78_slot04_00[obj->field_15a](obj);
}

int func_801b1208_slot04_00(Object *obj) {
    return 1;
}

int func_801b1210_slot04_00(Object *obj) {
    return obj->field_240 == 0;
}

int func_801b121c_slot04_00(Object *obj) {
    return obj->field_45 != 0;
}
