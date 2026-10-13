/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Poly28 data_801f4c40_slot06_03[2][0x48];

/* Draws the layer background_layers[1] as 24-cell rows of 16-pixel quads, the
   first row drawn cropped at the top by an offset (inferred). Forms that
   have to stay, each measured by undoing it alone on this function:
   - t holds the row offset before row and off are made from it: written
     twice from the layer, 15 instruction slots differ.
   - lim is assigned in the loop condition: as a plain expression the
     function is 4 bytes longer.
   - yu is a copy of ya made before the first-row test: stored as ya the
     function is 20 bytes shorter.
   - The second arm has a local of its own, yb2, for its top y: with yb used
     in both arms 19 instruction slots differ.
   - yb and yb2 are set in the inner loop's initialiser: as statements
     before the loop 4 instruction slots differ.
   - x is u16: as int the function is 20 bytes shorter. blank is s16: as
     u16 it is 8 bytes shorter. off is s16: as int 27 instruction slots
     differ. yb and yb2 are u16: as int the function is 20 bytes longer.
   - sx is declared before sy: swapped, 5 instruction slots differ. The
     first five locals (sx, sy, col, y, first) keep this order, which
     is their stack slot order.
   - The four x stores are four statements: chained two and two, 19
     instruction slots differ. */
void draw_tile_map_layer_polys_slot06_03(void) {
    u16 sx;
    u16 sy;
    u16 col;
    u16 y;
    u16 first;
    u16 mask;
    s16 blank;
    int n;
    int f12;
    int t;
    s16 row;
    s16 off;
    int lim;
    int i;
    int ya;
    u16 yu;
    u16 yb;
    u16 yb2;
    int k;
    u16 x;
    u16 cc;
    u16 tile;
    u16 *line;
    u16 *e;
    Poly28 *rec;
    u32 *ot;
    blank = ((Slot06Layer *)main_layer)->packed_param;
    n = ((Slot06Layer *)main_layer)->signed_param;
    f12 = (s16)((Slot06Layer *)main_layer)->scroll_x;
    sx = f12 & 0xf;
    y = ((Slot06Layer *)main_layer)->field_4c - (n + 5);
    sy = y & 0xf;
    col = (f12 & (((Slot06Layer *)main_layer)->map_width - 1)) >> 4;
    t = (s16)((Slot06Layer *)main_layer)->field_4c - 5;
    row = t / 16;
    off = t - (row << 4);
    mask = (((Slot06Layer *)main_layer)->map_width >> 4) - 1;
    first = 0;
    rec = data_801f4c40_slot06_03[draw_buffer_index];
    ot = &((u32 *)current_ordering_table)[14];
    for (i = (s16)y / 16; i < (lim = (s16)y / 16 + 15) - ((s16)((Slot06Layer *)main_layer)->field_4c - 5) / 16; i++, row++) {
        line = ((Slot06Layer *)main_layer)->tile_map_ptr + 0x800 + ((u16)row >> 4) * ((((Slot06Layer *)main_layer)->map_width >> 9) << 10) + ((row & 0xf) << 6);
        ya = sy + (i << 4);
        cc = col;
        yu = ya;
        if (first == 0) {
            for (k = 0, yb = ya + 16 - off; k < 24; k++) {
                e = line + (((u16)cc >> 5) << 10) + ((cc & 0x1f) << 1);
                tile = *e;
                if ((s16)tile != blank) {
                    x = (k << 4) - sx;
                    rec->tpage = (s16)tile >> 10;
                    rec->clut = e[1];
                    rec->x0 = x;
                    rec->x1 = x + 16;
                    rec->x2 = x;
                    rec->x3 = x + 16;
                    rec->y0 = rec->y1 = yu;
                    rec->y2 = rec->y3 = yb;
                    rec->u0 = rec->u2 = tile << 3;
                    rec->u1 = rec->u3 = (tile << 3) + 15;
                    rec->v0 = rec->v1 = off + (((s16)tile >> 2) & -16);
                    rec->v2 = rec->v3 = (((s16)tile >> 2) & -16) | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
            first = 0x88;
        } else {
            for (k = 0, yb2 = ya - off; k < 24; k++) {
                e = line + (((u16)cc >> 5) << 10) + ((cc & 0x1f) << 1);
                tile = *e;
                if ((s16)tile != blank) {
                    x = (k << 4) - sx;
                    rec->tpage = (s16)tile >> 10;
                    rec->clut = e[1];
                    rec->x0 = x;
                    rec->x1 = x + 16;
                    rec->x2 = x;
                    rec->x3 = x + 16;
                    rec->y0 = rec->y1 = yb2;
                    rec->y2 = rec->y3 = yb2 + 16;
                    rec->u0 = rec->u2 = tile << 3;
                    rec->u1 = rec->u3 = (tile << 3) + 15;
                    rec->v0 = rec->v1 = ((s16)tile >> 2) & -16;
                    rec->v2 = rec->v3 = (((s16)tile >> 2) & -16) | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
        }
    }
}
