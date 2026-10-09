/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801b8cbc_slot04_sel[])(void);

void func_801b4d8c_slot04_sel(Object *unused) {
    data_801b8cbc_slot04_sel[data_8018f5a0->field_50]();
}
