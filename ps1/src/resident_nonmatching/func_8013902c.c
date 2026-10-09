/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code uses other registers for two values (the second frame index
 * and a box y) and so differs in 26 instruction slots. The exact owner of
 * the bytes in the PS1 build stays the raw bytes of the original; the build
 * does not use this file. The differential test next to it
 * (func_8013902c.py, run with difftest.py) compares this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): collision push-out between
 * the two players. It first sets ref_third.p to player_left and the pointer
 * game_state.field_358 (the same word as ref_other.p) to player_right. Each
 * player has a frame record whose field_07 selects a box in the player's
 * table of 6-byte boxes (unknown_148); a field_07 of 0 means no box and the
 * function ends. The boxes are tested for overlap, first in y (box y minus
 * pos_y; the boxes overlap when the distance of the two is below the sum of
 * the box heights, byte at 5), then in x (box x, negated when the object's
 * field_0b is not 0, plus pos_x; overlap when the distance is below the sum
 * of the box widths, byte at 4). When they overlap in both, the overlap in
 * x is converted to a 16.16 fixed-point amount (overlap << 16) and added
 * to the 32-bit word at offset 0x10 (the fraction and pos_x) of one player
 * or, halved and in opposite directions, of both, to push them apart. Who
 * moves: if exactly one player has field_164 not 0, the other moves, in
 * the direction given by whether that field_164 equals 1; if both have it,
 * player_left moves (direction from player_right's field_164) unless
 * player_left's field_45 is 0 and player_right's is not, in which case
 * player_right moves (direction from player_left's field_164); if neither
 * has it, the one whose field_45 is not 0 moves when only one has it, the
 * direction being that of the sign of the x distance; otherwise both move
 * by half, away from each other.
 *
 * Contract:
 *   Argument: none. No result.
 *   Reads: player_left and player_right: frame (field_07), unknown_148
 *     table, pos_x, pos_y, field_0b, field_164, field_45, the word at 0x10;
 *     the box fields.
 *   Writes: ref_third.p, game_state.field_358, and the word at 0x10 of one
 *     or both players.
 *   Callees: none.
 *   Aliasing: the frame records and box tables are distinct blocks from
 *     the players; the two players are distinct.
 *   Excluded inputs: none. Every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef ref_third;

static void push(Object *o, s32 amount) {
    *(s32 *)&o->field_10 += amount;
}

void func_8013902c(void) {
    Object *o1;
    Object *o2;
    Box6 *b1;
    Box6 *b2;
    u8 i1;
    u8 i2;
    int dy;
    int dx;
    int x1;
    int x2;
    int far;
    int amount;

    ref_third.p = &player_left;
    game_state.field_358 = &player_right;
    o1 = ref_third.p;
    i1 = o1->frame->field_07;
    if (i1 == 0) return;
    o2 = ref_other.p;
    i2 = o2->frame->field_07;
    if (i2 == 0) return;
    b1 = (Box6 *)o1->unknown_148 + i1;
    b2 = (Box6 *)o2->unknown_148 + i2;

    dy = ((s16)b2->field_02 - o2->pos_y) - ((s16)b1->field_02 - o1->pos_y);
    if (dy < 0) dy = -dy;
    if (dy - (b1->field_05 + b2->field_05) >= 0) return;

    x1 = b1->origin;
    if (o1->field_0b != 0) x1 = -x1;
    x1 += o1->pos_x;
    x2 = b2->origin;
    if (o2->field_0b != 0) x2 = -x2;
    x2 += o2->pos_x;
    dx = x2 - x1;
    far = 0;
    if (dx < 0) {
        dx = -dx;
        far = 1;
    }
    dx -= b1->extent + b2->extent;
    if (dx >= 0) return;
    amount = -dx << 16;

    if (o1->field_164 != 0) {
        if (o2->field_164 == 0) {
            push(o2, o1->field_164 == 1 ? amount : -amount);
        } else if (o1->field_45 != 0) {
            push(o1, o2->field_164 == 1 ? amount : -amount);
        } else if (o2->field_45 != 0) {
            push(o2, o1->field_164 == 1 ? amount : -amount);
        } else {
            push(o1, o2->field_164 == 1 ? amount : -amount);
        }
    } else if (o2->field_164 != 0) {
        push(o1, o2->field_164 == 1 ? amount : -amount);
    } else if (o1->field_45 != 0 && o2->field_45 == 0) {
        push(o1, far ? amount : -amount);
    } else if (o1->field_45 == 0 && o2->field_45 != 0) {
        push(o2, far ? -amount : amount);
    } else {
        amount >>= 1;
        if (far && !(o1->pos_x < o2->pos_x)) {
            push(o1, amount);
            push(o2, -amount);
        } else {
            push(o1, -amount);
            push(o2, amount);
        }
    }
}
