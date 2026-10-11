/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_80015780_slot01[];

/* What it does (inferred, not an original name): turns a list of sprite
   parts into drawing primitives. The object's sequence step points at a
   description: a count, a list of 4-byte texture records and a list of
   4-byte position records. For each part it fills one 0x28-byte primitive
   with four corner positions (the part's position plus the object's
   pos_x and pos_y, and a size of 16 times the texture record's bytes 2
   and 3, the third and fourth byte) and four texture coordinates (from
   byte 0: its low nibble times 16 as u, its high nibble times 16 (the byte
   with its low nibble cleared) as v, each plus the size), then
   links the primitive into the ordering list whose index is
   data_80015780_slot01[object->field_09]. */
void func_80012dec_slot01(Object *obj, u8 *p) {
    Poly28 *prim = (Poly28 *)p;
    SpriteSet *shape = (SpriteSet *)obj->sequence->field_04;
    u32 idx = data_80015780_slot01[obj->field_09];
    int n = shape->count;
    u16 *xy = shape->offsets;
    u8 *d = shape->codes;
    u8 a;
    int x, y;
    u16 b;
    u16 c;
    u16 lo;
    u16 hi;

    do {
        a = *d;
        d += 2;
        b = *d++;
        c = *d++;
        x = *xy++;
        y = *xy++;
        n--;
        b <<= 4;
        c <<= 4;
        prim->field_08 = x + (u16)obj->pos_x;
        prim->field_0a = y + (u16)obj->pos_y;
        prim->field_10 = b + (x + (u16)obj->pos_x);
        prim->field_12 = y + (u16)obj->pos_y;
        prim->field_18 = x + (u16)obj->pos_x;
        prim->field_1a = c + (y + (u16)obj->pos_y);
        prim->field_20 = b + (x + (u16)obj->pos_x);
        prim->field_22 = c + (y + (u16)obj->pos_y);
        lo = a & 0xf;
        lo <<= 4;
        hi = a & 0xf0;
        prim->field_0c = lo;
        prim->field_0d = hi;
        prim->field_14 = lo + b;
        prim->field_15 = hi;
        prim->field_1c = lo;
        prim->field_1d = hi + c;
        prim->field_24 = lo + b;
        prim->field_25 = hi + c;
        ((PrimTag *)prim)->addr = ((PrimTag *)((u32 *)data_801987c8 + idx))->addr;
        ((PrimTag *)((u32 *)data_801987c8 + idx))->addr = (u32)prim;
        prim++;
    } while (n != 0);
}
