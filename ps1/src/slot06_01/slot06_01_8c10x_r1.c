/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_801f6b7c_slot06_01[2][0x84];

/* Draws a tile layer in perspective: for each row 44 cells, each cell that
   is not blank as a quad whose x offsets are interpolated between two values
   of the layer; the first row drawn is cropped at the top (inferred).
   The forms of this function are those of the model
   draw_skewed_tile_layer_quads_slot06_04, whose comment gives the measured effect of
   each; they were not measured again here. */
void draw_skewed_tile_layer_quads_slot06_01(void) {
    Slot06Layer *c = (Slot06Layer *)data_801aa5d4;
    u16 sx;
    u16 sy;
    u16 col;
    u16 first;
    u16 y;
    u16 mask;
    u16 sy2;
    int sy2s;
    int m16;
    s16 blank;
    s16 d1;
    s16 d2;
    int d1x;
    int d2x;
    s16 row;
    int k0;
    int n;
    int f12;
    int t;
    int k;
    int i;
    int ya;
    u16 yu;
    s16 yt;
    int y16;
    int lim;
    u16 cc;
    u16 tile;
    u16 *line;
    u16 *e;
    Poly28 *rec;
    u32 *ot;
    s32 a;
    s32 s0;
    s32 s1;
    blank = c->packed_param;
    k0 = -11;
    n = (s16)c->signed_param;
    y = c->field_4c - n;
    f12 = (s16)c->scroll_x;
    d1 = c->field_1c - n;
    sx = (f12 - 0xb0) & 0xf;
    sy = y & 0xf;
    col = (((s16)f12 - 0xb0) & (c->map_width - 1)) >> 4;
    d2 = c->rect_right - c->field_1c;
    row = (s16)c->field_4c / 16;
    mask = (c->map_width >> 4) - 1;
    t = (s16)c->field_4c - (row << 4);
    ot = &((u32 *)data_801987c8)[data_801aa544[1].ot_index - 1];
    sy2 = t;
    first = 0;
    rec = data_801f6b7c_slot06_01[data_801a27d0];
    for (i = (s16)y / 16, d1x = d1, d2x = d2, sy2s = (s16)sy2, m16 = (s16)sy2 - 16; i < (lim = (s16)y / 16 + 15) - (s16)c->field_4c / 16; i++, row++) {
        line = c->tile_map_ptr + ((u16)row >> 4) * ((c->map_width >> 9) << 10) + ((row & 0xf) << 6);
        ya = sy + (i << 4);
        cc = col;
        yu = ya;
        if (first == 0) {
            a = *(s32 *)&c->bound_x_b;
            a -= *(s32 *)&c->bound_x_a;
            s1 = a - a * ((u16)yu - d1x) / d2x;
            s0 = a - a * ((u16)yu - m16 - d1x) / d2x;
            for (k = k0; k < 33; k++) {
                e = line + (((u16)cc >> 5) << 10) + ((cc & 0x1f) << 1);
                tile = *e;
                if ((s16)tile != blank) {
                    int u = tile << 3;
                    int v = ((s16)tile >> 2) & -16;
                    Poly28 *p = rec;
                    u16 w;
                    p->tpage = (s16)tile >> 10;
                    w = e[1];
                    p->x0 = (k << 4) - sx + (s1 >> 16);
                    p->x1 = p->x0 + 16;
                    p->x2 = (k << 4) - sx + (s0 >> 16);
                    p->x3 = p->x2 + 16;
                    p->clut = w;
                    p->y0 = p->y1 = yu;
                    y16 = ya + 16;
                    p->y2 = p->y3 = y16 - sy2;
                    p->u0 = p->u2 = u;
                    p->u1 = p->u3 = u + 15;
                    p->v0 = p->v1 = sy2 + v;
                    p->v2 = p->v3 = v | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
            first = 0x88;
        } else {
            a = *(s32 *)&c->bound_x_b;
            a -= *(s32 *)&c->bound_x_a;
            s1 = a - a * ((u16)yu - sy2s - d1x) / d2x;
            s0 = a - a * ((u16)yu - m16 - d1x) / d2x;
            for (k = k0; k < 33; k++) {
                e = line + (((u16)cc >> 5) << 10) + ((cc & 0x1f) << 1);
                tile = *e;
                if ((s16)tile != blank) {
                    int u = tile << 3;
                    int v = ((s16)tile >> 2) & -16;
                    Poly28 *p = rec;
                    u16 w;
                    p->tpage = (s16)tile >> 10;
                    w = e[1];
                    p->x0 = (k << 4) - sx + (s1 >> 16);
                    p->x1 = p->x0 + 16;
                    p->x2 = (k << 4) - sx + (s0 >> 16);
                    p->x3 = p->x2 + 16;
                    p->clut = w;
                    yt = ya - sy2;
                    p->y0 = p->y1 = yt;
                    p->y2 = p->y3 = yt + 16;
                    p->u0 = p->u2 = u;
                    p->u1 = p->u3 = u + 15;
                    p->v0 = p->v1 = v;
                    p->v2 = p->v3 = v | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
        }
    }
}
