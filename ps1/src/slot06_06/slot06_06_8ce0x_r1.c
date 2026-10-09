/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_801f3618_slot06_06[2][0xb0];

/* Draws a tile layer in perspective: for each row 44 cells, each cell that
   is not blank as a quad whose x offsets are interpolated between two values
   of the layer; the first row drawn is cropped at the top (inferred).
   The forms that had to stay, and their measurements, are those of the
   model func_801e8bc4_slot06_04. */
void func_801e8ce0_slot06_06(void) {
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
    blank = c->field_1e;
    k0 = -11;
    n = (s16)c->field_3e;
    y = c->field_4c - n;
    f12 = (s16)c->field_12;
    d1 = c->field_1c - n;
    sx = (f12 - 0xb0) & 0xf;
    sy = y & 0xf;
    col = (((s16)f12 - 0xb0) & (c->field_58 - 1)) >> 4;
    d2 = c->field_4e - c->field_1c;
    row = (s16)c->field_4c / 16;
    mask = (c->field_58 >> 4) - 1;
    t = (s16)c->field_4c - (row << 4);
    ot = &((u32 *)data_801987c8)[data_801aa544[1].field_8a - 1];
    sy2 = t;
    first = 0;
    rec = data_801f3618_slot06_06[data_801a27d0];
    for (i = (s16)y / 16, d1x = d1, d2x = d2, sy2s = (s16)sy2, m16 = (s16)sy2 - 16; i < (lim = (s16)y / 16 + 15) - (s16)c->field_4c / 16; i++, row++) {
        line = c->field_50 + ((u16)row >> 4) * ((c->field_58 >> 9) << 10) + ((row & 0xf) << 6);
        ya = sy + (i << 4);
        cc = col;
        yu = ya;
        if (first == 0) {
            a = *(s32 *)&c->field_10;
            a -= *(s32 *)&c->field_08;
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
                    p->field_16 = (s16)tile >> 10;
                    w = e[1];
                    p->field_08 = (k << 4) - sx + (s1 >> 16);
                    p->field_10 = p->field_08 + 16;
                    p->field_18 = (k << 4) - sx + (s0 >> 16);
                    p->field_20 = p->field_18 + 16;
                    p->field_0e = w;
                    p->field_0a = p->field_12 = yu;
                    y16 = ya + 16;
                    p->field_1a = p->field_22 = y16 - sy2;
                    p->field_0c = p->field_1c = u;
                    p->field_14 = p->field_24 = u + 15;
                    p->field_0d = p->field_15 = sy2 + v;
                    p->field_1d = p->field_25 = v | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
            first = 0x88;
        } else {
            a = *(s32 *)&c->field_10;
            a -= *(s32 *)&c->field_08;
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
                    p->field_16 = (s16)tile >> 10;
                    w = e[1];
                    p->field_08 = (k << 4) - sx + (s1 >> 16);
                    p->field_10 = p->field_08 + 16;
                    p->field_18 = (k << 4) - sx + (s0 >> 16);
                    p->field_20 = p->field_18 + 16;
                    p->field_0e = w;
                    yt = ya - sy2;
                    p->field_0a = p->field_12 = yt;
                    p->field_1a = p->field_22 = yt + 16;
                    p->field_0c = p->field_1c = u;
                    p->field_14 = p->field_24 = u + 15;
                    p->field_0d = p->field_15 = v;
                    p->field_1d = p->field_25 = v | 15;
                    ((PrimTag *)rec)->addr = ((PrimTag *)ot)->addr;
                    ((PrimTag *)ot)->addr = (u32)rec;
                    rec++;
                }
                cc = mask & (cc + 1);
            }
        }
    }
}
