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
   data_801987c8[table_801e47a8_slot0b[object.field_09]]. */
void func_801e0ea8_slot0b(Object *obj, u8 *p) {
    Poly28 *prim = (Poly28 *)p;
    SpriteSet *shape = (SpriteSet *)obj->sequence->field_04;
    u32 idx = table_801e47a8_slot0b[obj->field_09];
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
