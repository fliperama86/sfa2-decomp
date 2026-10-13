/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void draw_tile_layer_0_16px_slot06_04(void);
void draw_tile_layer_sprites_with_partial_row_slot06_04(void);
void draw_tilemap_layer_32px_9x13_slot06_04(void);
void draw_skewed_tile_layer_quads_slot06_04(void);
void init_first_layer_tile_map_slot06_04(Slot06Layer *layer);
void init_layer_tile_map_580e_with_rect_slot06_04(Slot06Layer *layer);
void set_layer_tile_map_param_640c_width_scale_slot06_04(Slot06Layer *layer);
void set_layer_tile_map_param_580e_width_scale_slot06_04(Slot06Layer *layer);
void init_two_banks_112_tile_prims_gray_slot06_04(void);
void init_background_tile_prim_banks_slot06_04(void);
void init_two_banks_of_26_tile_prims_slot06_04(void);
void init_two_banks_of_poly_ft4_slot06_04(void);

void draw_background_layers_by_flags_slot06_04(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_04();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_04();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_04();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_04();
    }
}

void init_four_layers_tile_maps_slot06_04(void) {
    init_first_layer_tile_map_slot06_04((Slot06Layer *)background_layers);
    init_layer_tile_map_580e_with_rect_slot06_04((Slot06Layer *)main_layer);
    set_layer_tile_map_param_640c_width_scale_slot06_04((Slot06Layer *)cam_obj);
    set_layer_tile_map_param_580e_width_scale_slot06_04((Slot06Layer *)fourth_layer);
}

void init_gray_tile_background_sprite_poly_banks_slot06_04(void) {
    init_two_banks_112_tile_prims_gray_slot06_04();
    init_background_tile_prim_banks_slot06_04();
    init_two_banks_of_26_tile_prims_slot06_04();
    init_two_banks_of_poly_ft4_slot06_04();
}
