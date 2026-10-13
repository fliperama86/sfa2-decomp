/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_first_layer_tile_map_slot06_00(Slot06Layer *layer);
void init_second_layer_tile_map_slot06_00(Slot06Layer *layer);
void init_third_layer_tile_map_slot06_00(Slot06Layer *layer);
void init_two_tile_sprite_arrays_gray_slot06_00(void);
void init_large_tile_sprite_prims_slot06_00(void);
void draw_tile_layer_1_16px_slot06_00(void);
void draw_tilemap_layer_32px_9x13_slot06_00(void);
void noop_empty_function_2_slot06_00(void);
void noop_with_byte_pointer_slot06_00(u8 *p);
void noop_empty_function_3_slot06_00(void);
void init_tile_sprites_gray_2x114_slot06_00(void);
void draw_tile_layer_0_16px_slot06_00(void);

void draw_background_layers_by_flags_slot06_00(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_00();
    }
    if (*flags & 4) {
        draw_tile_layer_1_16px_slot06_00();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_00();
    }
    if (*flags & 4) {
        noop_empty_function_2_slot06_00();
    }
}

void init_background_layers_slot06_00(void) {
    init_first_layer_tile_map_slot06_00((Slot06Layer *)background_layers);
    init_second_layer_tile_map_slot06_00((Slot06Layer *)main_layer);
    init_third_layer_tile_map_slot06_00((Slot06Layer *)cam_obj);
    noop_with_byte_pointer_slot06_00(fourth_layer);
}

void init_gray_tile_and_large_sprite_prims_slot06_00(void) {
    init_tile_sprites_gray_2x114_slot06_00();
    init_two_tile_sprite_arrays_gray_slot06_00();
    init_large_tile_sprite_prims_slot06_00();
    noop_empty_function_3_slot06_00();
}
