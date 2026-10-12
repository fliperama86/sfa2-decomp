/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ea11c_slot06_0a[];
extern Slot06Tile data_801efed0_slot06_0a[2][0x124];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void toggle_layer_flag_and_update_camera_slot06_0a(Slot06Layer *layer);

void dispatch_layer_mode_then_save_cam_prev_pos_slot06_0a(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        toggle_layer_flag_and_update_camera_slot06_0a(layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
}

void toggle_layer_flag_and_update_camera_slot06_0a(Slot06Layer *layer) {
    layer->flags_bits ^= 0x800;
    if (layer->field_00 != 0 && game_state.side_bits_2 == 0) {
        update_camera_x_from_player_span((Sprite *)layer);
        update_cam_y_toward_players((Sprite *)layer);
    }
}

void init_layer_map_fields_stage_0a_a_slot06_0a(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea11c_slot06_0a;
    layer->data_table = data_801ea11c_slot06_0a;
    layer->packed_param = 0x5818;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_slot06_0a_tiles_gray_slot06_0a(void) {
    int i;
    int j;
    Slot06Tile *r;

    i = 0;
    do {
        j = 0;
        do {
            r = &data_801efed0_slot06_0a[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x124);
        i++;
    } while (i < 2);
}
