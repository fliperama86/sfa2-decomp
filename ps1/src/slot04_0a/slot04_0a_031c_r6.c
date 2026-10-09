/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c0578_slot04_0a[];

void func_801b0c88_slot04_0a(Object *obj) {
    data_801ad398 = data_801c0578_slot04_0a[obj->field_15a](obj);
}

int func_801b0cd0_slot04_0a(Object *obj) {
    return 1;
}

int func_801b0cd8_slot04_0a(Object *obj) {
    return obj->field_240 == 0;
}
