/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in the order of a few loads
 * and constants and in the order of the operands of one address sum. The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * original; the build does not use this file. The differential test next
 * to it (func_80138d70.py, run with difftest.py) compares this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): when state->field_30 is 0,
 * it picks one entry, a pair of bytes, of the table data_80176da0 (index:
 * 64 * field_2d + 16 * row + 2 * field_54, 16-bit signed, where row is 3
 * when field_42 read as a signed halfword is 4 or more and otherwise
 * field_42 itself as an unsigned halfword, so a negative field_42 is not
 * clamped), and builds a 32-byte pattern: all bytes 0x40, then the first
 * n1 bytes set to 0 (n1 is the first byte of the entry), then the next n2
 * bytes set to 0x20 (n2 is the second byte). A random byte (func_80151184,
 * masked to 0..31) selects one byte of the pattern; it is stored into
 * field_220 of each player (player_left, player_right) whose field_cd is not 0.
 *
 * Contract:
 *   Argument: a0 = pointer to the game state (the prototype's type). No
 *     result. Returns at once, writing nothing, when field_30 is not 0.
 *   Reads: state->field_30, field_2d, field_42, field_54; two bytes of
 *     data_80176da0 at the computed index; player_left.field_cd and
 *     player_right.field_cd; the generator's state (data_80190126).
 *   Writes: the generator's state (through func_80151184), and field_220 of
 *     each player whose field_cd is not 0.
 *   Callees: func_80151184 runs as the original code in both runs (it is a
 *     pseudo-random generator that updates the halfword data_80190126 and
 *     returns its low byte; inferred); no recorder.
 *   Aliasing: the game state and the two players are distinct blocks.
 *   Excluded inputs: the two table bytes of the entry are each at most 16,
 *     so their sum is at most 32; a larger sum writes beyond the 32-byte
 *     pattern into the stack frame, which the original's frame can take (up
 *     to 48) but is not a contract of the C. The table and the index stay
 *     inside a block that the setup fills (the index can reach 1070 bytes
 *     above the table, 1071 for its second byte, and 64 below it).
 *   Unreached slots: none; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138d70(GameState *state) {
    u8 pattern[32];
    int i;
    int j;
    int zeros;
    int fills;
    int row;
    s16 index;
    int value;

    if (state->field_30 != 0) return;
    row = state->field_42;
    if ((s16)state->field_42 >= 4) row = 3;
    index = state->field_54 * 2 + (row * 16 + state->field_2d * 64);
    for (i = 31; i >= 0; i--) pattern[i] = 0x40;
    zeros = data_80176da0[index];
    for (i = 0; i < zeros; i++) pattern[i] = 0;
    fills = data_80176da0[index + 1];
    for (j = 0; j < fills; j++) pattern[i + j] = 0x20;
    value = pattern[func_80151184() & 0x1f];
    if (player_left.field_cd != 0) player_left.field_220 = value;
    if (player_right.field_cd != 0) player_right.field_220 = value;
}
