/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void copy_table_pair_to_two_entries(u8 *a, u8 *b, u8 index) {
    a[0x18] = data_801721c8[index];
    b[0x18] = data_801721c8[index];
    a[0x19] = data_801721c8[index + 1];
    b[0x19] = data_801721c8[index + 1];
    data_80190474 = 0;
}

void set_wait_flag(GameState *a, u8 v) {
    a->wait_flag = v;
}
