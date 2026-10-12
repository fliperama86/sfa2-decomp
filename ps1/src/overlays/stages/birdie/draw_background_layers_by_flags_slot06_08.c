/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ea6d8_slot06_08[];
extern u16 data_80190542;

void init_first_layer_tile_map_slot06_08(Slot06Layer *layer);
void init_layer_map_fields_with_rect_slot06_08(Slot06Layer *layer);
void set_layer_tile_map_and_scale_f_slot06_08(Slot06Layer *layer);
void init_layer_map_fields_slot06_08(Slot06Layer *layer);
void init_slot06_08_two_tile_groups_gray_slot06_08(void);
void init_slot06_08_tiles_gray_slot06_08(void);
void init_sprite_buffers_32x32_gray_slot06_08(void);
void init_poly_ft4_buffers_gray_slot06_08(void);
void draw_tile_layer_0_16px_slot06_08(void);
void draw_scrolling_tile_map_layer_slot06_08(void);
void draw_tilemap_layer_32px_9x13_slot06_08(void);
void build_perspective_layer_tile_quads_slot06_08(void);
void init_sprite_at_1c0_zero(Sprite *sprite);

void draw_background_layers_by_flags_slot06_08(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_08();
    }
    if (*flags & 4) {
        draw_scrolling_tile_map_layer_slot06_08();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_08();
    }
    if (*flags & 4) {
        build_perspective_layer_tile_quads_slot06_08();
    }
}

void init_four_layers_tile_maps_slot06_08(void) {
    init_first_layer_tile_map_slot06_08((Slot06Layer *)data_801aa544);
    init_layer_map_fields_with_rect_slot06_08((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_and_scale_f_slot06_08((Slot06Layer *)cam_obj);
    init_layer_map_fields_slot06_08((Slot06Layer *)data_801904d8);
}

void init_gray_tile_groups_sprites_poly_ft4_slot06_08(void) {
    init_slot06_08_two_tile_groups_gray_slot06_08();
    init_slot06_08_tiles_gray_slot06_08();
    init_sprite_buffers_32x32_gray_slot06_08();
    init_poly_ft4_buffers_gray_slot06_08();
}

void update_layer_position_by_mode_slot06_08(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_sprite_at_1c0_zero((Sprite *)layer);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
        d += data_80190542;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e += layer->field_0e;
        e += layer->pos_y_int;
        layer->field_26 = e;
    }
}

void init_first_layer_tile_map_slot06_08(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea6d8_slot06_08;
    layer->data_table = data_801ea6d8_slot06_08;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
