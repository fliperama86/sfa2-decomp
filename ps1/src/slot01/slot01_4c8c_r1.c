/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01Rec33bb4 data_80033bb4_slot01[2][2500];
void init_draw_mode_prim_with_merge_slot01(Tx *tx);

void init_primitive_table_and_load_images_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    int i;
    int j;
    Slot01Rec33bb4 *r;
    Rect rect;

    o->vel_y = 0x80093000;
    o->cycle_value = 0x5800;
    o->accel_y = 0x400;
    *(u32 *)&o->health = 0x100;
    o->state_0++;
    obj->pos_y = 0;
    obj->ot_index = 0x10;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x118; j++) {
            r = &data_80033bb4_slot01[i][j];
            init_draw_mode_prim_with_merge_slot01((Tx *)r);
            r->color_r = 0x80;
            r->color_g = 0x80;
            r->color_b = 0x80;
        }
    }
    rect.x = 0;
    rect.y = 0x1e0;
    rect.w = 0x10;
    rect.h = 0x20;
    func_80157fc4(&rect, (u8 *)0x80095000);
    rect.x = 0x2c0;
    rect.y = 0x100;
    rect.w = 0x40;
    rect.h = 0x100;
    func_80157fc4(&rect, (u8 *)0x80095400);
    func_80157d9c(0);
}

void init_draw_mode_prim_with_merge_slot01(Tx *tx) {
    func_80158a2c((Prim *)tx, 0, 0, 0, 0);
    tx->mode_id = 3;
    tx->prim_code = 0x74;
    func_8015c23c(tx, &tx->prim_data);
}
