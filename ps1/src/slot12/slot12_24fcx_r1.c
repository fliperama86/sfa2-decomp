/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Tiles *data_80018d80_slot12[];
extern SlotCell data_80029350_slot12[];

/* The read of value_b into t stands before the test of field_64: written after that test, this function differs from the original in 13 instruction slots. */
void add_tile_map_quads_for_frame_slot12(Object *obj, FrameRecord *frames) {
    FrameRecord *f = &frames[obj->sequence->frame_index];
    int t;
    u16 cx;
    int d;
    int y;
    int ytop0;
    int ybot0;
    Slot12Tiles *tile;
    SlotCell *fp;
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

    t = f->value_b;
    if (game_state.field_64 != 0) {
        return;
    }
    t--;
    if (t == -1) {
        return;
    }
    tile = data_80018d80_slot12[t];
    h = tile->height;
    w = tile->width;
    if (w * h == 0) {
        return;
    }
    idx = 0;
    cx = obj->pos_x - box_margin[0];
    xmin = cx - tile->half_width;
    xmax = cx + tile->half_width;
    xb = xmax - 0x10;
    xa = xmin + 0x10;
    fp = data_80029350_slot12 + data_801a27d0 * 0x34 + obj->field_02 * 26;
    d = tile->tile_value - 0xc8;
    y = data_801aa5ea[0] - d;
    list = (u16 *)(tile + 1);
    ytop0 = y - 0x20;
    ybot0 = y - 0x10;
    arg = (int)(data_801987c8 + 0x48);
    flip = obj->facing_side;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            v = list[idx++];
            if (v != 0) {
                tx = v & 0xf;
                ty = (v & 0xf0) >> 4;
                if (!(v & 0x8000)) {
                    fp->tex0_u = tx << 4;
                    fp->tex0_v = ty << 4;
                    fp->tex1_v = ty << 4;
                    fp->tex1_u = (tx << 4) + 0xf;
                    fp->tex2_u = tx << 4;
                    fp->tex2_v = (ty << 4) + 0xf;
                    fp->tex3_u = (tx << 4) + 0xf;
                    fp->tex3_v = (ty << 4) + 0xf;
                } else {
                    fp->tex0_u = (tx << 4) + 0xf;
                    fp->tex0_v = ty << 4;
                    fp->tex1_v = ty << 4;
                    fp->tex1_u = tx << 4;
                    fp->tex2_u = (tx << 4) + 0xf;
                    fp->tex2_v = (ty << 4) + 0xf;
                    fp->tex3_u = tx << 4;
                    fp->tex3_v = (ty << 4) + 0xf;
                }
                if (!(flip & 1)) {
                    fp->vertex0_x = xmin + col * 0x10;
                    fp->vertex0_y = ytop0 + row * 0x10;
                    fp->vertex1_x = xa + col * 0x10;
                    fp->vertex1_y = ytop0 + row * 0x10;
                    fp->vertex2_x = xmin + col * 0x10;
                    fp->vertex2_y = ybot0 + row * 0x10;
                    fp->vertex3_x = xa + col * 0x10;
                    fp->vertex3_y = ybot0 + row * 0x10;
                } else {
                    fp->vertex0_x = xmax - col * 0x10;
                    fp->vertex0_y = ytop0 + row * 0x10;
                    fp->vertex1_x = xb - col * 0x10;
                    fp->vertex1_y = ytop0 + row * 0x10;
                    fp->vertex2_x = xmax - col * 0x10;
                    fp->vertex2_y = ybot0 + row * 0x10;
                    fp->vertex3_x = xb - col * 0x10;
                    fp->vertex3_y = ybot0 + row * 0x10;
                }
                func_8015bf34(arg, (Cmd *)fp++);
            }
        }
    }
}
