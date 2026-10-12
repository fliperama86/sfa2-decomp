/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801e9b7c_slot06_0b[];

void init_sprite_at_1c0_zero(Sprite *sprite);
void init_first_layer_tile_map_slot06_0b(Slot06Layer *layer);
void init_layer_tile_map_fields_with_rect_slot06_0b(Slot06Layer *layer);
void init_layer_tile_map_fields_slot06_0b(Slot06Layer *layer);
void set_layer_tile_map_param_580e_width_scale_slot06_0b(Slot06Layer *layer);
void draw_tile_layer_0_16px_slot06_0b(void);
void draw_tile_layer_sprites_with_partial_row_slot06_0b(void);
void draw_tilemap_layer_32px_9x13_slot06_0b(void);
void draw_skewed_tile_layer_quads_slot06_0b(void);
void init_slot06_0b_two_tile_groups_gray_slot06_0b(void);
void init_slot06_0b_tiles_gray_slot06_0b(void);
void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_0b(void);
void init_layer_poly_ft4_prims_slot06_0b(void);

void draw_background_layers_by_flags_slot06_0b(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_0b();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_0b();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_0b();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_0b();
    }
}

void init_four_layers_tile_maps_slot06_0b(void) {
    init_first_layer_tile_map_slot06_0b((Slot06Layer *)data_801aa544);
    init_layer_tile_map_fields_with_rect_slot06_0b((Slot06Layer *)data_801aa5d4);
    init_layer_tile_map_fields_slot06_0b((Slot06Layer *)cam_obj);
    set_layer_tile_map_param_580e_width_scale_slot06_0b((Slot06Layer *)data_801904d8);
}

void init_slot_tile_groups_and_layer_poly_gray_h_slot06_0b(void) {
    init_slot06_0b_two_tile_groups_gray_slot06_0b();
    init_slot06_0b_tiles_gray_slot06_0b();
    init_two_banks_of_91_gray_32x32_sprite_prims_slot06_0b();
    init_layer_poly_ft4_prims_slot06_0b();
}

void update_layer_position_from_main_layer_scaled_slot06_0b(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    int e;

    if (layer->mode == 0) {
        init_sprite_at_1c0_zero((Sprite *)layer);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = data_801aa5f6;
        d -= l2->ref_x;
        if (d != 0) {
            e = (s16)l2->rect_right - (s16)l2->field_1c;
            d = d * (e - 0x11) / e;
        }
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        d = data_801aa5fa;
        d -= l2->field_0e;
        d += layer->field_0e;
        d += layer->pos_y_int;
        layer->field_26 = d;
    }
}

void init_first_layer_tile_map_slot06_0b(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e9b7c_slot06_0b;
    layer->data_table = data_801e9b7c_slot06_0b;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
