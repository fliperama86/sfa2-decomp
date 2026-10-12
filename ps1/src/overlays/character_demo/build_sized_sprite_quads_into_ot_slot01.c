/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

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
   data_80015780_slot01[object->ot_depth]. */
void build_sized_sprite_quads_into_ot_slot01(Object *obj, u8 *p) {
    Poly28 *prim = (Poly28 *)p;
    SpriteSet *shape = (SpriteSet *)obj->sequence->data_ptr;
    u32 idx = data_80015780_slot01[obj->ot_depth];
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
        prim->x0 = x + (u16)obj->pos_x;
        prim->y0 = y + (u16)obj->pos_y;
        prim->x1 = b + (x + (u16)obj->pos_x);
        prim->y1 = y + (u16)obj->pos_y;
        prim->x2 = x + (u16)obj->pos_x;
        prim->y2 = c + (y + (u16)obj->pos_y);
        prim->x3 = b + (x + (u16)obj->pos_x);
        prim->y3 = c + (y + (u16)obj->pos_y);
        lo = a & 0xf;
        lo <<= 4;
        hi = a & 0xf0;
        prim->u0 = lo;
        prim->v0 = hi;
        prim->u1 = lo + b;
        prim->v1 = hi;
        prim->u2 = lo;
        prim->v2 = hi + c;
        prim->u3 = lo + b;
        prim->v3 = hi + c;
        ((PrimTag *)prim)->addr = ((PrimTag *)((u32 *)data_801987c8 + idx))->addr;
        ((PrimTag *)((u32 *)data_801987c8 + idx))->addr = (u32)prim;
        prim++;
    } while (n != 0);
}
