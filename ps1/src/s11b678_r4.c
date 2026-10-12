/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void init_cd(void) {
    func_8015c6ec();
}

void set_table_entry_and_global_pointer(int *out, int index) {
    func_80157d9c(0);
    out[0] = table_8016e6b8[index];
    out[1] = func_8015789c();
}
