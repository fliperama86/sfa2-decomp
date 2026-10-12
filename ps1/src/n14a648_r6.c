/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 call_script_subop_table_and_return_one(Object *unused) {
    data_8017d368[data_80189468 >> 1]();
    return 1;
}

u8 call_script_subop_table_and_return_one_2(Object *unused) {
    data_8017d3b4[data_80189468 >> 1]();
    return 1;
}
