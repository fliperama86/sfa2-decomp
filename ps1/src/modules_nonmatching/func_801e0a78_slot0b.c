/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 556 bytes against the original's 624: the original has
 * two copies of the x scaling, one for each side of the mirror test, and
 * this C has one; register choice and instruction order differ too. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): fills a run of 0x28-byte
 * polygon records with the corners of a list of squares and links each into
 * an ordering-table list. A layout reached through the object's sequence
 * holds a count and a list of coordinate pairs. Each pair is transformed by
 * the object's state: field_48 mirrors x (x becomes -x - 16), field_20 and
 * field_22 scale x and y about the point 8 (value + ((value + 8) * scale >>
 * 4)), and field_24 (its low byte indexes a table of sine and cosine
 * pairs) rotates about the point (-8, -8) with 8 fractional bits. The
 * square then has its corners at the result plus the object's position,
 * 16 wide and 16 high. The records are written from `buf`, or from
 * `buf + count` when the fourth argument is not 0 (the second half of a
 * double buffer).
 *
 * Contract:
 *   Arguments: a0 = object, a1 = first record of an array of 2 * count
 *     records, a2 unused, a3 = flag. No return value.
 *   Reads: object field_09, pos_x, pos_y, field_20, field_22, field_24,
 *     field_48; sequence->field_04 (the layout: s16 count at 0, the
 *     coordinate list at 8, u16 pairs); table_801e1fec_slot0b entries
 *     (s16 at 0 and 2) indexed by the low byte of field_24; the list base
 *     pointer data_801987c8 and the word 8 + field_09 words into it.
 *   Writes: per record offsets 0x08, 0x0a, 0x10, 0x12, 0x18, 0x1a, 0x20,
 *     0x22 and the link word at 0; the list word.
 *   No callee.
 *   Aliasing: the object, sequence step, layout, coordinate list, the
 *     rotation table, the record array and the list array are distinct
 *     blocks.
 *   Excluded inputs: a count of 0 or less (the loop counts down to zero and
 *     would run for 2^32 turns); the setup uses 1 to 6.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0bRot table_801e1fec_slot0b[];

void func_801e0a78_slot0b(Object *obj, Poly28 *buf, int unused, int flag) {
    Slot0bLayout *lay = (Slot0bLayout *)obj->sequence->field_04;
    int n = lay->count;
    u16 *coords = lay->coords;
    Poly28 *p = buf;
    u32 *ot;
    Slot0bRot *rot;
    int x;
    int y;
    int rx;

    if (flag != 0) {
        p = buf + n;
    }
    do {
        x = *coords++;
        y = *coords++;
        if (obj->field_48 != 0) {
            x = -x - 16;
        }
        if (obj->field_20 != 0) {
            x += (((s16)x + 8) * (s16)obj->field_20) >> 4;
        }
        if (obj->field_22 != 0) {
            y += (((s16)y + 8) * (s16)obj->field_22) >> 4;
        }
        if (obj->field_24 != 0) {
            rot = table_801e1fec_slot0b + *(u8 *)&obj->field_24;
            rx = ((rot->s * (s16)(x + 8)) >> 8) + (((s16)rot->c * (s16)(y + 8)) >> 8) - 8;
            y = ((-(s16)rot->c * (s16)(x + 8)) >> 8) + ((rot->s * (s16)(y + 8)) >> 8) - 8;
            x = rx;
        }
        p->field_08 = x + obj->pos_x;
        p->field_0a = y + obj->pos_y;
        p->field_10 = x + obj->pos_x + 16;
        p->field_12 = y + obj->pos_y;
        p->field_18 = x + obj->pos_x;
        p->field_1a = y + obj->pos_y + 16;
        p->field_20 = x + obj->pos_x + 16;
        p->field_22 = y + obj->pos_y + 16;
        ot = (u32 *)data_801987c8 + obj->field_09 + 8;
        ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)p;
        p++;
        n--;
    } while (n != 0);
}
