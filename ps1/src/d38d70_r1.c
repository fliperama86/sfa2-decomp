/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Fills one row of 32 bytes with a value (inferred). */
static inline void row_fill(u8 *row, u8 value) {
    int k;
    for (k = 31; k >= 0; k--) {
        row[k] = value;
    }
}

/*
 * What it does (inferred, not an original name): when state->versus_flag is 0,
 * it picks a pair of bytes from data_80176da0 (indexed by table_index_2,
 * count_index clamped to 3, and sub_index) and builds a 32-byte pattern: all
 * 0x40, then the first byte of the pair as a count of 0 bytes, then the
 * second as a count of 0x20 bytes. A random byte (get_random_byte, masked
 * to 0..31) selects one pattern byte, stored into list_offset of each player
 * whose mode_flag_2 is not 0.
 */
void func_80138d70(GameState *state) {
    u8 pattern[40];
    int i;
    int j;
    int count;
    short index;
    int value;
    int base;

    if (state->versus_flag == 0) {
        j = state->table_index_2;
        base = j * 64;
        count = state->count_index;
        if ((short)state->count_index >= 4) {
            count = 3;
        }
        j = state->sub_index;
        index = j * 2 + (count * 16 + base);
        row_fill(pattern, 0x40);
        count = data_80176da0[index];
        i = 0;
        if (count != 0) {
            for (i = 0; i < count; i++) {
                pattern[i] = 0;
            }
        }
        count = data_80176da0[index + 1];
        if (count != 0) {
            for (j = 0; j < count; j++) {
                /* Plain spelling pattern[i + j] differs in 10 instruction lines; (pattern + i)[j] in 1. */
                ((u8 *)(i + (int)pattern))[j] = 0x20;
            }
        }
        value = pattern[get_random_byte() & 0x1f];
        if (player_left.mode_flag_2 != 0) {
            player_left.list_offset = value;
        }
        if (player_right.mode_flag_2 != 0) {
            player_right.list_offset = value;
        }
    }
}
