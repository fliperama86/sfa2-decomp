/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_layer_tile_map_5800_conditional_slot06_06(Slot06Layer *layer);
void init_layer_tile_map_6000_with_rect_slot06_06(Slot06Layer *layer);
void func_801e9440_slot06_06(Slot06Layer *layer);
void func_801e92b0_slot06_06(Slot06Layer *layer);
void draw_tile_layer_0_16px_slot06_06(void);
void draw_tile_map_layer_sprites_interpolated_slot06_06(void);
void draw_tilemap_layer_32px_9x13_slot06_06(void);
void draw_skewed_tile_layer_quads_slot06_06(void);
void func_801e8710_slot06_06(void);
void func_801e946c_slot06_06(void);
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
    func_801e9440_slot06_06((Slot06Layer *)cam_obj);
    func_801e92b0_slot06_06((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_06(void) {
    init_layer_tile_prims_two_arrays_slot06_06();
    func_801e8710_slot06_06();
    func_801e946c_slot06_06();
    init_layer_poly_ft4_prims_slot06_06();
}
