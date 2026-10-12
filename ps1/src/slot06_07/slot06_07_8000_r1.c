/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_first_layer_tile_map_slot06_07(Slot06Layer *layer);
void func_801e8610_slot06_07(Slot06Layer *layer);
void func_801e9378_slot06_07(Slot06Layer *layer);
void func_801e91dc_slot06_07(Slot06Layer *layer);
void func_801e822c_slot06_07(void);
void init_slot06_07_tiles_gray_slot06_07(void);
void func_801e93a4_slot06_07(void);
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
    func_801e8610_slot06_07((Slot06Layer *)data_801aa5d4);
    func_801e9378_slot06_07((Slot06Layer *)cam_obj);
    func_801e91dc_slot06_07((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_07(void) {
    func_801e822c_slot06_07();
    init_slot06_07_tiles_gray_slot06_07();
    func_801e93a4_slot06_07();
    init_two_banks_of_poly_ft4_slot06_07();
}
