/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c1ea0_slot04_07[];
extern ObjectFn data_801c1ec8_slot04_07[];

void func_801b1960_slot04_07(Object *obj) {
    data_801ad398 = data_801c1ea0_slot04_07[obj->field_15a](obj);
}

int return_true_always_slot04_07(Object *obj) {
    return 1;
}

int func_801b19b0_slot04_07(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_remaining_count_slot04_07(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b19d0_slot04_07(Object *obj) {
    data_801c1ec8_slot04_07[obj->field_15a](obj);
}
