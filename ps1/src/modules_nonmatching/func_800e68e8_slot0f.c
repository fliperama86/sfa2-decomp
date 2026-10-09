/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and
 * instruction order. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (func_800e68e8_slot0f.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): fills two consecutive
 * sprite records, p and p + 1 (0x20 bytes each), with the same content.
 * A sprite number n selects a table of 16-bit values: n == 2 uses the entry
 * of data_800efef8_slot0f chosen by the object's field_48, any other n uses
 * entry n of data_800efdc8_slot0f. The first two values are the texture
 * coordinates; the values from index 6 on are two 16-bit numbers handed to
 * func_8015bd0c, whose result (low 5 bits) goes into the command word with
 * two bytes of the block data_801987c8 points at (shifted by 9 and 10).
 * The position is the object's pos_x plus entry n of data_800eff58_slot0f,
 * and the object's pos_y.
 *
 * Contract (what the code reads and writes; the roles named are inferred):
 *   Arguments: a0 = object, a1 = pointer to the first of two records,
 *     a2 = n (only its low 16 bits are used, as unsigned).
 *   No return value.
 *   Reads: obj->pos_x, obj->pos_y, obj->field_48 (n == 2 only),
 *     data_800efdc8_slot0f[n] (n != 2) or data_800efef8_slot0f[field_48],
 *     the halfwords of the table it points at (indices 0, 1, 6, 7),
 *     data_800eff58_slot0f[n], and from the block data_801987c8 points at
 *     the bytes at 0xa2 and 0xa3.
 *   Writes: in each of the two records the command word (offset 4), x, y,
 *     u (low byte of the first halfword), v (low byte of the second) and
 *     the texture page (0x7f07). Nothing else.
 *   Callee: func_8015bd0c(0, 0, s16 first, s16 second) is replaced by a
 *     recorder (it reaches Sony's library) and returns a random value per
 *     case. The log copies both records (16 words) at the call.
 *   Aliasing: object, records, tables and the data_801987c8 block are
 *     distinct blocks.
 *   Inputs excluded: none beyond n < 76 for the two table sizes read here
 *     (n above reads image data that is not a table of pointers).
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_800efdc8_slot0f[];
extern u16 *data_800efef8_slot0f[];
extern u16 data_800eff58_slot0f[];

void func_800e68e8_slot0f(Object *obj, Slot12Prim *p, int n) {
    Slot12Prim *q = p + 1;
    u16 *t;
    u16 u;
    u16 v;
    int r;
    u32 cmd;

    if ((u16)n != 2) {
        t = data_800efdc8_slot0f[(u16)n];
    } else {
        t = data_800efef8_slot0f[obj->field_48];
    }
    u = t[0];
    v = t[1];
    p->u = u;
    p->v = v;
    q->u = u;
    q->v = v;
    p->x = obj->pos_x + data_800eff58_slot0f[(u16)n];
    p->y = obj->pos_y;
    q->x = obj->pos_x + data_800eff58_slot0f[(u16)n];
    q->y = obj->pos_y;
    p->tpage = 0x7f07;
    q->tpage = 0x7f07;
    r = func_8015bd0c(0, 0, (s16)t[6], (s16)t[7]);
    cmd = 0xe1000000 + (((Object *)data_801987c8)->field_a2 << 9) + (((Object *)data_801987c8)->field_a3 << 10) + (r & 0x1f);
    p->cmd = cmd;
    q->cmd = cmd;
}
