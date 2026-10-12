/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_sprite_set_textured_quads_slot01(Object *obj, u8 *a, int b, int c, int d);

/* What it does (inferred, not an original name): fills textured-quad
   primitives for a set of sprites, twice over the same set (the second
   pass continues in the primitive array where the first stopped). The
   object's sequence step points at a set: an s16 count, a list of codes
   (bytes) and a list of offsets (pairs of u16). For each sprite it
   initialises the primitive through func_8015c09c, sets the three colour
   bytes to 0x80, the texture page, a clut-like halfword (base plus the
   code times 64) and the four corner texture coordinates: the offset
   plus (0x60, 0x50), a square of 16. When the flag is not 0 the square is
   mirrored in x: the left pair of texture x coordinates gets the offset +
   0x6f and the right pair the offset + 0x60, instead of the offset + 0x60
   and the offset + 0x70. A set with a count of 0 or less
   still runs its loop once (the loop is a do-while). */
void init_sprite_set_textured_quads_slot01(Object *obj, u8 *a, int b, int c, int d) {
    Poly28 *prim = (Poly28 *)a;
    u16 tpage = b;
    /* bs is a copy of the argument c (u16 and int give the same code); using c directly differs from the original in 14 instruction slots. */
    u16 bs = c;
    int fl = d;
    SpriteSet *set = (SpriteSet *)obj->sequence->data_ptr;
    int i;
    int n;
    u8 *codes;
    u16 *offs;
    int x;
    int y;
    u16 xr;
    u16 z;
    Poly28 *p;

    for (i = 0; i < 2; i++) {
        n = set->count;
        codes = set->codes;
        offs = set->offsets;
        if (fl == 0) {
            /* p is a second pointer: the original has one register for the call's argument and one for the stores; with the stores through prim and prim++ at the loop's end this function differs from the original in 2 instruction slots (measured with both loops written that way). */
            p = prim;
            do {
                func_8015c09c(prim);
                prim++;
                n--;
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->tpage = tpage;
                x = *offs++;
                y = *offs++;
                x += 0x60;
                y += 0x50;
                p->u0 = x;
                /* Stand-in, not recovered source: the test below, which is never true (its value is 0 for every x), and its arm reproduce the original's order of instructions. Without it this function differs from the original in 2 instruction slots. */
                if ((((u32)x << 24) & 0x100)) {
                    x = 0;
                }
                xr = x + 0x10;
                p->u2 = x;
                p->v0 = y;
                p->u1 = xr;
                p->v1 = y;
                p->v2 = y + 0x10;
                p->u3 = xr;
                p->v3 = y + 0x10;
                p->clut = bs + (*codes++ << 6);
                p++;
            } while (n > 0);
        } else {
            /* The second pointer p again, as above (the figure above is for both loops). */
            p = prim;
            do {
                func_8015c09c(prim);
                prim++;
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->tpage = tpage;
                x = *offs++;
                y = *offs++;
                y += 0x50;
                z = x + 0x60;
                p->u0 = z + 0xf;
                p->u2 = z + 0xf;
                p->v0 = y;
                p->u1 = z;
                p->v1 = y;
                p->v2 = y + 0x10;
                p->u3 = z;
                p->v3 = y + 0x10;
                p->clut = bs + (*codes++ << 6);
                p++;
                n--;
            } while (n > 0);
        }
    }
}
