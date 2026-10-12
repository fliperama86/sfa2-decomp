/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea938_slot06_05[];
extern Slot06Tile data_801edf0c_slot06_05[2][0x175];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);

void update_layer_by_mode_then_cam_slot06_05(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
}

void func_801e863c_slot06_05(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea938_slot06_05;
    layer->data_table = data_801ea938_slot06_05;
    layer->packed_param = 0x580a;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e8668_slot06_05(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801edf0c_slot06_05[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x175);
        i++;
    } while (i < 2);
}
