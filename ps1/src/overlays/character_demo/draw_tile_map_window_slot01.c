/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot01Rec33bb4 data_80033bb4_slot01[2][2500];

void draw_tile_map_window_slot01(Object *obj) {
    Object *o = obj;
    s16 w = *(u32 *)&o->health >> 3;
    s16 px = *(u16 *)&o->pos_x;
    int ox = px & 7;
    int t = -(((Slot01Obj *)o)->pos_y + -0x100000) >> 16;
    int oy = t & 7;
    s16 cell = o->cycle_value;
    int x0 = (px & (((Slot01Obj *)o)->width - 1)) >> 3;
    s16 y = t;
    s16 row;
    Slot01Rec33bb4 *p;
    u32 *ot;
    int i;
    int j;
    int x;
    u8 *base;
    u16 *src;
    s16 c;

    row = y >= 0 ? y >> 3 : (y - 7) / 8;
    p = data_80033bb4_slot01[draw_buffer_index];
    ot = (u32 *)current_ordering_table + ((Slot01Obj *)o)->ot_index;
    for (i = 0; i < 10; i++) {
        x = x0;
        if (row >= 0 && row < w) {
            base = (u8 *)(o->vel_y + ((u16)row >> 5) * ((((Slot01Obj *)o)->width >> 9) << 12) * 2 + ((row & 0x1f) << 8));
            for (j = 0; j < 0x30; j++, x++) {
                if ((u16)x < 0x40) {
                    src = (u16 *)(base + ((s16)x / 64 << 13) + ((x & 0x3f) << 2));
                    if (src[0] != cell) {
                        c = src[0];
                        p->x = j * 8 - ox;
                        p->y = i * 8 - oy;
                        p->u = (c << 3) & -8;
                        p->v = (c >> 2) & -8;
                        p->tpage = src[1];
                        p->cmd = 0xe100001b;
                        ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
                        ((PrimTag *)ot)->addr = (u32)p;
                        p++;
                    }
                }
            }
        }
        row++;
    }
}
