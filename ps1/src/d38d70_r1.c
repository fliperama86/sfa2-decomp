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
 * What it does (inferred, not an original name): when state->field_30 is 0,
 * it picks a pair of bytes from data_80176da0 (indexed by field_2d,
 * field_42 clamped to 3, and field_54) and builds a 32-byte pattern: all
 * 0x40, then the first byte of the pair as a count of 0 bytes, then the
 * second as a count of 0x20 bytes. A random byte (get_random_byte, masked
 * to 0..31) selects one pattern byte, stored into field_220 of each player
 * whose field_cd is not 0.
 */
void func_80138d70(GameState *state) {
    u8 pattern[40];
    int i;
    int j;
    int count;
    short index;
    int value;
    int base;

    if (state->field_30 == 0) {
        j = state->field_2d;
        base = j * 64;
        count = state->field_42;
        if ((short)state->field_42 >= 4) {
            count = 3;
        }
        j = state->field_54;
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
        if (player_left.field_cd != 0) {
            player_left.field_220 = value;
        }
        if (player_right.field_cd != 0) {
            player_right.field_220 = value;
        }
    }
}
