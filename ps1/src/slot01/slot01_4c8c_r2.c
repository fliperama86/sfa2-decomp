/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01Rec33bb4 data_80033bb4_slot01[2][2500];

void func_80014e24_slot01(Object *obj) {
    Object *o = obj;
    s16 w = *(u32 *)&o->field_5c >> 3;
    s16 px = *(u16 *)&o->pos_x;
    int ox = px & 7;
    int t = -(((Slot01Obj *)o)->field_14 + -0x100000) >> 16;
    int oy = t & 7;
    s16 cell = o->field_1e;
    int x0 = (px & (((Slot01Obj *)o)->field_58 - 1)) >> 3;
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
    p = data_80033bb4_slot01[data_801a27d0];
    ot = (u32 *)data_801987c8 + ((Slot01Obj *)o)->field_8a;
    for (i = 0; i < 10; i++) {
        x = x0;
        if (row >= 0 && row < w) {
            base = (u8 *)(o->field_50 + ((u16)row >> 5) * ((((Slot01Obj *)o)->field_58 >> 9) << 12) * 2 + ((row & 0x1f) << 8));
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
