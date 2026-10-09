/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Tiles *data_80018d80_slot12[];
extern SlotCell data_80029350_slot12[];
extern u16 box_margin[];
extern u16 data_801aa5ea[];

void func_800124fc_slot12(Object *obj, FrameRecord *frames) {
    FrameRecord *f = &frames[obj->sequence->frame_index];
    int t;
    u16 cx;
    int d;
    int g;
    int y;
    int ytop0;
    int ybot0;
    Slot12Tiles *tile;
    SlotCell *fp;
    SlotCell *c;
    u16 idx;
    int w;
    int row;
    int col;
    unsigned int v;
    int tx;
    int ty;
    int arg;
    u16 xb;
    u16 xmin;
    u16 xmax;
    u16 xa;
    u8 flip;
    u16 h;
    u16 *list;

    t = f->field_05;
    if (game_state.field_64 != 0) {
        return;
    }
    t = t - 1;
    if (t == -1) {
        return;
    }
    tile = data_80018d80_slot12[t];
    h = tile->field_02;
    w = tile->field_00;
    if (w * h == 0) {
        return;
    }
    idx = 0;
    g = data_801a27d0;
    cx = (u16)obj->pos_x - box_margin[0];
    xmin = cx - tile->field_04;
    xmax = cx + tile->field_04;
    xb = xmax - 0x10;
    xa = xmin + 0x10;
    fp = data_80029350_slot12 + g * 0x34 + obj->field_02 * 26;
    d = tile->field_06 - 0xc8;
    y = data_801aa5ea[0] - d;
    list = (u16 *)(tile + 1);
    ytop0 = y - 0x20;
    ybot0 = y - 0x10;
    arg = (int)(data_801987c8 + 0x48);
    flip = obj->field_0b;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            v = list[idx++];
            if (v != 0) {
                tx = v & 0xf;
                ty = (v & 0xf0) >> 4;
                if (!(v & 0x8000)) {
                    fp->field_0c = tx << 4;
                    fp->field_0d = ty << 4;
                    fp->field_15 = ty << 4;
                    fp->field_14 = (tx << 4) + 0xf;
                    fp->field_1c = tx << 4;
                    fp->field_1d = (ty << 4) + 0xf;
                    fp->field_24 = (tx << 4) + 0xf;
                    fp->field_25 = (ty << 4) + 0xf;
                } else {
                    fp->field_0c = (tx << 4) + 0xf;
                    fp->field_0d = ty << 4;
                    fp->field_15 = ty << 4;
                    fp->field_14 = tx << 4;
                    fp->field_1c = (tx << 4) + 0xf;
                    fp->field_1d = (ty << 4) + 0xf;
                    fp->field_24 = tx << 4;
                    fp->field_25 = (ty << 4) + 0xf;
                }
                if (!(flip & 1)) {
                    fp->field_08 = xmin + col * 0x10;
                    fp->field_0a = ytop0 + row * 0x10;
                    fp->field_10 = xa + col * 0x10;
                    fp->field_12 = ytop0 + row * 0x10;
                    fp->field_18 = xmin + col * 0x10;
                    fp->field_1a = ybot0 + row * 0x10;
                    fp->field_20 = xa + col * 0x10;
                    fp->field_22 = ybot0 + row * 0x10;
                } else {
                    fp->field_08 = xmax - col * 0x10;
                    fp->field_0a = ytop0 + row * 0x10;
                    fp->field_10 = xb - col * 0x10;
                    fp->field_12 = ytop0 + row * 0x10;
                    fp->field_18 = xmax - col * 0x10;
                    fp->field_1a = ybot0 + row * 0x10;
                    fp->field_20 = xb - col * 0x10;
                    fp->field_22 = ybot0 + row * 0x10;
                }
                func_8015bf34(arg, (Cmd *)fp++);
            }
        }
    }
}
