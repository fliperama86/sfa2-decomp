/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_layer_tile_map_5800_conditional_slot06_06(Slot06Layer *layer);
void init_layer_tile_map_6000_with_rect_slot06_06(Slot06Layer *layer);
void set_layer_tile_map_param_6400_width_scale_slot06_06(Slot06Layer *layer);
void set_layer_tile_map_param_6000_width_scale_slot06_06(Slot06Layer *layer);
void draw_tile_layer_0_16px_slot06_06(void);
void draw_tile_map_layer_sprites_interpolated_slot06_06(void);
void draw_tilemap_layer_32px_9x13_slot06_06(void);
void draw_skewed_tile_layer_quads_slot06_06(void);
void init_two_banks_75_tile_prims_gray_slot06_06(void);
void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_06(void);
void init_layer_poly_ft4_prims_slot06_06(void);
void init_layer_tile_prims_two_arrays_slot06_06(void);

void draw_background_layers_by_flags_slot06_06(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_06();
    }
    if (*flags & 4) {
        draw_tile_map_layer_sprites_interpolated_slot06_06();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_06();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_06();
    }
}

void init_four_layers_tile_maps_slot06_06(void) {
    init_layer_tile_map_5800_conditional_slot06_06((Slot06Layer *)data_801aa544);
    init_layer_tile_map_6000_with_rect_slot06_06((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_param_6400_width_scale_slot06_06((Slot06Layer *)cam_obj);
    set_layer_tile_map_param_6000_width_scale_slot06_06((Slot06Layer *)data_801904d8);
}

void init_slot_layer_prim_buffers_gray_e_slot06_06(void) {
    init_layer_tile_prims_two_arrays_slot06_06();
    init_two_banks_75_tile_prims_gray_slot06_06();
    init_two_banks_of_91_gray_32x32_sprite_prims_slot06_06();
    init_layer_poly_ft4_prims_slot06_06();
}
