/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_first_layer_tile_map_slot06_02(Slot06Layer *layer);
void init_layer_with_map_801eaf18_and_rect_slot06_02(Slot06Layer *layer);
void set_layer_tile_map_and_scale_d_slot06_02(Slot06Layer *layer);
void set_layer_tile_map_and_scale_c_slot06_02(Slot06Layer *layer);
void init_two_tile_arrays_gray_color_slot06_02(void);
void init_two_banks_282_tile_prims_gray_slot06_02(void);
void init_sprite_buffers_13_32x32_gray_slot06_02(void);
void init_poly_ft4_buffers_gray_slot06_02(void);
void draw_tile_map_layer_16px_slot06_02(void);
void draw_tile_layer_sprites_with_partial_row_slot06_02(void);
void draw_tile_map_layer_32px_slot06_02(void);
void draw_skewed_tile_layer_quads_slot06_02(void);
void init_layer_sprite_then_advance_mode_slot06_02(Slot06Layer *layer);
void update_layer_follow_and_flag_blink_slot06_02(Slot06Layer *layer);

void draw_background_layers_by_flags_slot06_02(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_map_layer_16px_slot06_02();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_02();
    }
    if (*flags & 8) {
        draw_tile_map_layer_32px_slot06_02();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_02();
    }
}

void init_four_layers_tile_maps_slot06_02(void) {
    init_first_layer_tile_map_slot06_02((Slot06Layer *)data_801aa544);
    init_layer_with_map_801eaf18_and_rect_slot06_02((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_and_scale_d_slot06_02((Slot06Layer *)cam_obj);
    set_layer_tile_map_and_scale_c_slot06_02((Slot06Layer *)data_801904d8);
}

void init_all_gray_prim_groups_four_calls_a_slot06_02(void) {
    init_two_tile_arrays_gray_color_slot06_02();
    init_two_banks_282_tile_prims_gray_slot06_02();
    init_sprite_buffers_13_32x32_gray_slot06_02();
    init_poly_ft4_buffers_gray_slot06_02();
}

void call_layer_handler_by_mode_slot06_02(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_layer_sprite_then_advance_mode_slot06_02(layer);
    } else if (layer->mode == 1) {
        update_layer_follow_and_flag_blink_slot06_02(layer);
    }
}

void init_layer_sprite_then_advance_mode_slot06_02(Slot06Layer *layer) {
    init_sprite_position((Sprite *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_8b = 3;
        layer->mode++;
    }
}
