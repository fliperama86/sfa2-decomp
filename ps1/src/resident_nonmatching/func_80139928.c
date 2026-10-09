/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs in the allocation of two registers (the cursor's width
 * and its y coordinate sit in each other's registers). The exact owner of
 * the bytes in the PS1 build stays the raw bytes of the original; the build
 * does not use this file. The differential test next to it
 * (func_80139928.py, run with difftest.py) compares this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): tests whether the box of
 * object b, placed at b's position, is separated from the cursor box of
 * object a, placed at a's position, and publishes the placement it used.
 * Both boxes are four halfwords: x, y, width, height. The cursor box is the
 * one game_state.cursor points at. When an object's field_0b is not 0 its
 * box is mirrored in x (the x of the box is negated). The x test runs
 * first: the boxes are apart in x when the 16-bit distance of the two
 * placed x values exceeds the sum of the widths (compared as signed 16-bit
 * values); then 1 is returned without looking at y. Otherwise the same test
 * on y, with the heights, decides the result. All sums and differences are
 * taken modulo 2^16.
 *
 * Contract:
 *   Arguments: a0 = object a, a1 = object b, a2 = pointer to b's box (a
 *     Box32 in the prototype; only its first four halfwords are read).
 *     Result in v0: 1 when apart, 0 when overlapping.
 *   Reads: game_state.cursor and the four halfwords it points at; both
 *     objects' field_0b, pos_x and pos_y; the four halfwords of the box.
 *   Writes: data_80188ed0.out_3c, out_40 (always: the placed x values of a
 *     and b), and out_44, out_48 (only when the x test did not return: the
 *     placed y values minus the box y values).
 *   Aliasing: objects, box and the cursor box are distinct blocks; a and b
 *     are distinct.
 *   Excluded inputs: none. Every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80139928(Object *a, Object *b, Box32 *box) {
    Quad16 *cursor;
    u16 ax;
    u16 bx;
    u16 pa;
    u16 pb;
    s16 distance;

    cursor = (Quad16 *)game_state.cursor;
    bx = cursor->v[0];
    ax = box->origin;
    if (a->field_0b != 0) bx = -bx;
    if (b->field_0b != 0) ax = -ax;

    pa = bx + a->pos_x;
    pb = ax + b->pos_x;
    data_80188ed0.out_3c = pa;
    data_80188ed0.out_40 = pb;
    distance = pb - pa;
    if (distance < 0) distance = -distance;
    if ((s16)(box->endpoint + cursor->v[2]) < distance) return 1;

    pa = a->pos_y - cursor->v[1];
    pb = b->pos_y - box->field_02;
    data_80188ed0.out_44 = pa;
    data_80188ed0.out_48 = pb;
    distance = pb - pa;
    if (distance < 0) distance = -distance;
    return (s16)(box->field_06 + cursor->v[3]) < distance;
}
