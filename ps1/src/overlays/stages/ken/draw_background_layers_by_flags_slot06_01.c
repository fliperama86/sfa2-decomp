/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_first_layer_tile_map_slot06_01(Slot06Layer *layer);
void init_layer_tile_map_with_rect_slot06_01(Slot06Layer *layer);
void set_layer_tile_map_and_scale_b_slot06_01(Slot06Layer *layer);
void set_layer_tile_map_and_scale_a_slot06_01(Slot06Layer *layer);
void draw_tilemap_layer_16px_12x25_slot06_01(void);
void draw_tile_layer_sprites_with_partial_row_slot06_01(void);
void draw_tilemap_layer_32px_7x13_slot06_01(void);
void draw_skewed_tile_layer_quads_slot06_01(void);
void init_tile_sprites_gray_2x275_slot06_01(void);
void init_two_tile_sprite_arrays_gray_b_slot06_01(void);
void init_sprite_buffers_32x32_gray_slot06_01(void);
void init_poly_ft4_buffers_gray_slot06_01(void);

void draw_background_layers_by_flags_slot06_01(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tilemap_layer_16px_12x25_slot06_01();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_01();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_7x13_slot06_01();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_01();
    }
}

void init_four_layers_tile_maps_slot06_01(void) {
    init_first_layer_tile_map_slot06_01((Slot06Layer *)data_801aa544);
    init_layer_tile_map_with_rect_slot06_01((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_and_scale_b_slot06_01((Slot06Layer *)cam_obj);
    set_layer_tile_map_and_scale_a_slot06_01((Slot06Layer *)data_801904d8);
}

void init_gray_tiles_sprites_and_poly_ft4_slot06_01(void) {
    init_tile_sprites_gray_2x275_slot06_01();
    init_two_tile_sprite_arrays_gray_b_slot06_01();
    init_sprite_buffers_32x32_gray_slot06_01();
    init_poly_ft4_buffers_gray_slot06_01();
}
