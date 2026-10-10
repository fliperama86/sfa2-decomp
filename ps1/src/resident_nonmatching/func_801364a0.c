/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 4 bytes larger and differs in instruction choice (a
 * register copy at the join of the three arms). The exact owner of the bytes
 * in the PS1 build stays the
 * raw bytes of the original; the build does not use this file. The
 * differential test next to it (func_801364a0.py, run with difftest.py)
 * compares this C with the original code on random inputs of the contract
 * below.
 *
 * What it does (inferred, not an original name): one step of a horizontal
 * scroll follower. It finds the span [lo, hi] that the players cover: each
 * player covers pos_x - field_154 to pos_x + field_154. If player_left has
 * bit 7 of field_73 set and field_261 equal to 0, the span is
 * player_right's; else if player_right has that state, the span is
 * player_left's; else the span is the union (lowest lo and highest hi, as
 * signed 16-bit values). When the span is 0xc0 or more wide (16-bit signed
 * difference), the follower aims at the span's middle: the amount
 * ((lo + hi) >> 1, from the 16-bit sum) - sprite->field_22 - 0xc0, as a
 * 16-bit signed value, drives func_801366d4 when negative, func_80136668
 * otherwise. When the span is narrower, the follower works on the edges: if
 * hi - field_22 - 0x120 is not negative it calls func_80136668 with it;
 * else, if lo - field_22 - 0x60 is not positive it calls func_801366d4 with
 * it; else it calls func_80136898 with field_22. The two edge amounts are
 * also 16-bit signed values.
 *
 * Contract:
 *   Argument: a0 = pointer to a sprite (the prototype's type). No result.
 *   Reads: player_left and player_right (field_73, field_261, pos_x,
 *     field_154); the sprite's field_22 and, through the callees, its
 *     other fields.
 *   Writes: what the callees write (the sprite's field_22; inferred).
 *   Callees: func_80136668, func_801366d4 and func_80136898 run as the
 *     original code in both runs (they are short game code that reads and
 *     writes the sprite's halfwords 0x22 to 0x46, inferred); no recorder.
 *   Aliasing: the sprite and the two players are distinct blocks.
 *   Excluded inputs: none. Every instruction slot is executed.
 * The tree declares func_80136898 with a Cam pointer; the sprite is cast to it here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Callees of the follower; not in the published prototypes. Inferred. */
void func_80136668(Cam *cam, short step);
void func_801366d4(Cam *cam, short step);

void func_801364a0(Sprite *sprite) {
    u16 lo;
    u16 hi;
    u16 other;
    s16 amount;

    if ((player_left.field_73 & 0x80) && player_left.field_261 == 0) {
        lo = player_right.pos_x - player_right.field_154;
        hi = player_right.pos_x + player_right.field_154;
    } else if ((player_right.field_73 & 0x80) && player_right.field_261 == 0) {
        lo = player_left.pos_x - player_left.field_154;
        hi = player_left.pos_x + player_left.field_154;
    } else {
        lo = player_left.pos_x - player_left.field_154;
        other = player_right.pos_x - player_right.field_154;
        if ((s16)other < (s16)lo) lo = other;
        hi = player_left.pos_x + player_left.field_154;
        other = player_right.pos_x + player_right.field_154;
        if ((s16)other > (s16)hi) hi = other;
    }
    if ((s16)(hi - lo) >= 0xc0) {
        amount = ((s16)(hi + lo) >> 1) - sprite->field_22 - 0xc0;
        if (amount < 0) func_801366d4((Cam *)sprite, amount);
        else func_80136668((Cam *)sprite, amount);
    } else {
        amount = hi - sprite->field_22 - 0x120;
        if (amount >= 0) {
            func_80136668((Cam *)sprite, amount);
        } else {
            amount = lo - sprite->field_22 - 0x60;
            if (amount <= 0) func_801366d4((Cam *)sprite, amount);
            else func_80136898((Cam *)sprite, (s16)sprite->field_22);
        }
    }
}
