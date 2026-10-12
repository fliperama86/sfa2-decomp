/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_first_layer_tile_map_slot06_07(Slot06Layer *layer);
void set_layer_tile_map_param_580e_and_rect_bounds_slot06_07(Slot06Layer *layer);
void set_layer_tile_map_param_6808_width_scale_slot06_07(Slot06Layer *layer);
void set_layer_tile_map_param_580e_width_scale_slot06_07(Slot06Layer *layer);
void init_gray_tile_banks_66_and_152_prims_slot06_07(void);
void init_slot06_07_tiles_gray_slot06_07(void);
void init_two_banks_of_81_gray_32x32_sprite_prims_slot06_07(void);
void init_two_banks_of_poly_ft4_slot06_07(void);
void draw_tile_layer_0_16px_slot06_07(void);
void draw_tile_layer_sprites_with_partial_row_slot06_07(void);
void draw_tilemap_layer_32px_9x13_slot06_07(void);
void draw_skewed_tile_layer_quads_slot06_07(void);

void draw_background_layers_by_flags_slot06_07(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_07();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_07();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_07();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_07();
    }
}

void init_four_layers_tile_maps_slot06_07(void) {
    init_first_layer_tile_map_slot06_07((Slot06Layer *)data_801aa544);
    set_layer_tile_map_param_580e_and_rect_bounds_slot06_07((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_param_6808_width_scale_slot06_07((Slot06Layer *)cam_obj);
    set_layer_tile_map_param_580e_width_scale_slot06_07((Slot06Layer *)data_801904d8);
}

void init_slot_tile_sprite_and_poly_buffers_gray_f_slot06_07(void) {
    init_gray_tile_banks_66_and_152_prims_slot06_07();
    init_slot06_07_tiles_gray_slot06_07();
    init_two_banks_of_81_gray_32x32_sprite_prims_slot06_07();
    init_two_banks_of_poly_ft4_slot06_07();
}
