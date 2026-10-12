/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eadc0_slot06_11[];
extern Slot06Tile data_801efe80_slot06_11[2][0x190];
extern Slot06Tile data_801f620c_slot06_11[2][0x100];

void func_801362f4(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);

void update_layer_by_mode_then_cam_and_scroll_slot06_11(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_801362f4((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void func_801e8568_slot06_11(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eadc0_slot06_11;
    layer->data_table = data_801eadc0_slot06_11;
    layer->packed_param = 0x580e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_tile_groups_gray_slot06_11(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Tile *t;
    Slot06Sprt16 *sp;
    Slot06Sprt16 *q;

    i = 0;
    do {
        j = 0;
        do {
            r = (Slot06Tile *)((u8 *)data_801efe80_slot06_11 + i * 0x2bc0 + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x190);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f620c_slot06_11[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            init_draw_mode_prim_3((Tx *)(i * 0x1c00 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x1c00) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x100);
        i++;
    } while (i < 2);
}
