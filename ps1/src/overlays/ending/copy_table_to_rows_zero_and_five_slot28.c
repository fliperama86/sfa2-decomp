/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_800516ec_slot28[];

void copy_table_to_rows_zero_and_five_slot28(void) {
    int i;
    for (i = 0; i < 0x30; i++) {
        data_801a27e4_rows[0][i] = data_800516ec_slot28[i];
        data_801a27e4_rows[5][i] = data_800516ec_slot28[i];
    }
}
