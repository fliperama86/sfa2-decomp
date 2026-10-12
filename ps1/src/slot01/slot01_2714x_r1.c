/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_800212d8_slot01[];

/* What it does (inferred, not an original name): fills quad primitives for
   the sprites of a set, one per sprite, and links each into an ordering
   list. The object's sequence step points at a set: an s16 count and a
   list of offsets (pairs of u16 x, y). Each offset is transformed in
   steps: when the object's field_48 is not 0 the x is mirrored (-x - 16);
   a non-zero field_20 then stretches x about -8 (x grows by ((x + 8) *
   field_20) >> 4, an arithmetic shift of the signed product), a non-zero
   field_22 does the same for y; a non-zero field_24 selects a 2x2 matrix
   of s16 entries from the table data_800212d8_slot01 (index = the low byte
   of field_24, 4 entries each), and (x + 8, y + 8) is multiplied by it, in
   1/256 units, minus 8. The four corners are then the position plus the
   object's pos_x, pos_y and a square of 16. A non-zero flag starts filling
   the primitive array at entry count instead of at entry 0. */
void func_80012714_slot01(Object *obj, u8 *prim_arg, int unused, int flag) {
    Poly28 *prim = (Poly28 *)prim_arg;
    s16 *t;
    SpriteSet *set = (SpriteSet *)obj->sequence->field_04;
    int count = set->count;
    u16 *offs = set->offsets;
    Poly28 *p = prim;
    int n;
    int a;
    u32 *ot;
    int b;

    if (flag != 0) {
        p += count;
    }
    ot = (u32 *)data_801987c8;
    /* Stand-in, not recovered source: the term that is ORed to ot below, which is 0 for every value of prim, reproduces the original's choice of registers. Without it this function differs from the original in 88 instruction slots. */
    ot = (u32 *)((u8 *)ot + (((u32)(unsigned long long)prim << 24) & 0x100));
    do {
        int x = *offs++;
        int y = *offs++;
        if (obj->field_48 == 0) {
            if (obj->field_20 != 0) {
                x += ((s16)x + 8) * (s16)obj->field_20 >> 4;
            }
        } else {
            n = -x - 0x10;
            /* Stand-in, not recovered source: the term that is ORed to x below, which is 0 for every value of n, reproduces the original's choice of registers. Without it this function differs from the original in 26 instruction slots. */
            x = n | (((u32)n << 24) & 0x100);
            if (obj->field_20 != 0) {
                x = n + (((s16)n + 8) * (s16)obj->field_20 >> 4);
            }
        }
        if (obj->field_22 != 0) {
            y += ((s16)y + 8) * (s16)obj->field_22 >> 4;
        }
        if (obj->field_24 != 0) {
            t = &data_800212d8_slot01[(u8)obj->field_24 * 4];
            a = (s16)(x + 8);
            b = (s16)(y + 8);
            /* The table is read through one pointer that moves; with the four entries indexed directly and no pointer step the function differs from the original in 7 instruction slots. */
            x = *t++ * a;
            y = *t++ * b;
            x >>= 8;
            y >>= 8;
            x += y;
            x -= 8;
            y = (t[0] * a >> 8) + (t[1] * b >> 8) - 8;
        }
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        /* Without it this function differs from the original in 36 instruction slots. */
        /* The four nested wrappers are one stand-in. The figure is the one above: all four taken out, the statements inside them kept. */
        do {
            do {
                do {
                    do {
                        /* Stand-in, not recovered source: one stand-in at 5 places, the five statements below, each of which ORs to x or to y a term that is 0 for every value of it, reproduces the original's choice of registers. Without it this function differs from the original in 19 instruction slots. */
                        x |= (((u32)x << 24) & 0x100);
                        x |= (((u32)x << 24) & 0x100);
                        y |= (((u32)y << 24) & 0x100);
                        y |= (((u32)y << 24) & 0x100);
                        y |= (((u32)y << 24) & 0x100);
                    } while (0);
                } while (0);
                p->field_08 = x + (u16)obj->pos_x;
                p->field_0a = y + (u16)obj->pos_y;
                p->field_10 = x + (u16)obj->pos_x + 0x10;
                p->field_12 = y + (u16)obj->pos_y;
                p->field_18 = x + (u16)obj->pos_x;
                p->field_1a = y + (u16)obj->pos_y + 0x10;
                p->field_20 = x + (u16)obj->pos_x + 0x10;
                p->field_22 = y + (u16)obj->pos_y + 0x10;
                ((PrimTag *)p)->addr = ((PrimTag *)(ot + (obj->field_09 + 8)))->addr;
                ((PrimTag *)(ot + (obj->field_09 + 8)))->addr = (u32)p;
                /* One local for two values: p is moved on through t; with p++ the function differs from the original in 52 instruction slots. */
                t = (s16 *)(p + 1);
                p = (Poly28 *)t;
            } while (0);
        } while (0);
    } while (--count != 0);
}
