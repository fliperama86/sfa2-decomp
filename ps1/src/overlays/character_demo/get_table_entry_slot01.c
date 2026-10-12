/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80015680_slot01[];

int get_table_entry_slot01(u8 index) {
    return data_80015680_slot01[index];
}
