/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_first_layer_tile_map_slot06_03(Slot06Layer *layer);
void init_layer_map_and_sprite_colors_slot06_03(Slot06Layer *layer);
void set_layer_tile_map_and_scale_f_slot06_03(Slot06Layer *layer);
void set_layer_tile_map_and_scale_e_slot06_03(Slot06Layer *layer);
void init_two_banks_170_tile_prims_gray_slot06_03(void);
void init_two_banks_288_tile_prims_gray_slot06_03(void);
void init_two_banks_of_gray_32x32_sprite_prims_slot06_03(void);
void init_poly_ft4_banks_gray_slot06_03(void);
void draw_tile_map_layer_16px_slot03_slot06_03(void);
void draw_tile_layer_sprites_saving_pos_slot06_03(void);
void draw_tilemap_layer_32px_7x13_slot06_03(void);
void draw_tile_map_layer_polys_slot06_03(void);
void update_layer_motion_and_follow_scroll_slot06_03(Slot06Layer *layer);

void draw_background_layers_by_flags_slot06_03(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_map_layer_16px_slot03_slot06_03();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_saving_pos_slot06_03();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_7x13_slot06_03();
    }
    if (*flags & 4) {
        draw_tile_map_layer_polys_slot06_03();
    }
}

void init_four_layers_tile_maps_slot06_03(void) {
    init_first_layer_tile_map_slot06_03((Slot06Layer *)data_801aa544);
    init_layer_map_and_sprite_colors_slot06_03((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_and_scale_f_slot06_03((Slot06Layer *)cam_obj);
    set_layer_tile_map_and_scale_e_slot06_03((Slot06Layer *)data_801904d8);
}

void init_gray_tile_sprite_and_poly_banks_set_two_slot06_03(void) {
    init_two_banks_170_tile_prims_gray_slot06_03();
    init_two_banks_288_tile_prims_gray_slot06_03();
    init_two_banks_of_gray_32x32_sprite_prims_slot06_03();
    init_poly_ft4_banks_gray_slot06_03();
}

void call_handler_by_layer_mode_slot03_slot06_03(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_counted((Sprite *)layer, 0x190, 0x100);
    } else if (layer->mode == 1) {
        update_layer_motion_and_follow_scroll_slot06_03(layer);
    }
}
