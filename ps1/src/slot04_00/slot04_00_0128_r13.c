/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bfda0_slot04_00[];
extern ObjectFn data_801bfdc8_slot04_00[];

int func_801b1250_slot04_00(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_remaining_count_slot04_00(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1270_slot04_00(Object *obj) {
    data_801bfda0_slot04_00[obj->field_15a](obj);
}

void func_801b12b0_slot04_00(Object *obj) {
    data_801bfdc8_slot04_00[obj->field_15a](obj);
}
