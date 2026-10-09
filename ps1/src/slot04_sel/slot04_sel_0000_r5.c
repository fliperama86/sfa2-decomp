/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801b6e88_slot04_sel[])(void);

void func_801b165c_slot04_sel(void) {
}

void func_801b1664_slot04_sel(Object *obj) {
}

void func_801b166c_slot04_sel(Object *unused) {
    data_801b6e88_slot04_sel[data_8018f5a0->field_50]();
}
