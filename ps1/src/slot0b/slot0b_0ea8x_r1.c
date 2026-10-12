/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 table_801e47a8_slot0b[];

/* What it does (inferred, not an original name): fills a run of 0x28-byte
   polygon records with the textured rectangles of a shape and links each
   into an ordering-table list. A shape reached through the object's
   sequence holds a count, a list of 4-byte descriptors and a list of
   coordinate pairs. For each entry: a descriptor byte 0 holds the texture
   column in its low nibble and the row in its high nibble (each in units of
   16), bytes 2 and 3 the width and height in units of 16 (byte 1 is not
   used). The four corners are the entry's coordinate pair plus the object's
   position, widened by width and height; the texture coordinates are formed
   the same way. The record is linked into the list head
   data_801987c8[table_801e47a8_slot0b[object.ot_depth]]. */
void func_801e0ea8_slot0b(Object *obj, u8 *p) {
    Poly28 *prim = (Poly28 *)p;
    SpriteSet *shape = (SpriteSet *)obj->sequence->data_ptr;
    u32 idx = table_801e47a8_slot0b[obj->ot_depth];
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
