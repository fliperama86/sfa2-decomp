/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801b6d6c_slot04_sel[])(void);

void func_801b0244_slot04_sel(Object *unused) {
    data_801b6d6c_slot04_sel[data_8018f5a0->field_50]();
}
