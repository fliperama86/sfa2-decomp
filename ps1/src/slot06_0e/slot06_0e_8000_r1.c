/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eb8ec_slot06_0e[];
extern u16 data_80190542;

void init_first_layer_tile_map_slot06_0e(Slot06Layer *layer);
void init_layer_tile_map_0e_with_bounds_slot06_0e(Slot06Layer *layer);
void init_layer_tile_map_0e_front_slot06_0e(Slot06Layer *layer);
void init_layer_tile_map_fields_b_slot06_0e(Slot06Layer *layer);
void init_two_tile_prim_arrays_gray_slot06_0e(void);
void init_tile_prims_gray_two_sets_slot06_0e(void);
void init_two_banks_of_gray_32x32_sprite_prims_slot06_0e(void);
void init_poly_ft4_buffers_gray_slot06_0e(void);
void draw_tile_map_16px_tiles_slot06_0e(void);
void draw_tile_layer_sprites_with_partial_row_slot06_0e(void);
void draw_tile_map_layer_sprites_32px_slot06_0e(void);
void draw_perspective_skewed_tile_map_slot06_0e(void);
int init_layer_position_and_advance_mode_on_flag_slot06_0e(Slot06Layer *layer);
void update_layer_by_state_with_parallax_slot06_0e(Slot06Layer *layer);
void update_layer_start_or_scroll_down_slot06_0e(Slot06Layer *layer);
void advance_layer_pos_y_to_limit_slot06_0e(Slot06Layer *layer);

void draw_background_layers_by_flags_slot06_0e(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_map_16px_tiles_slot06_0e();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_0e();
    }
    if (*flags & 8) {
        draw_tile_map_layer_sprites_32px_slot06_0e();
    }
    if (*flags & 4) {
        draw_perspective_skewed_tile_map_slot06_0e();
    }
}

void init_four_layers_tile_maps_slot06_0e(void) {
    init_first_layer_tile_map_slot06_0e((Slot06Layer *)data_801aa544);
    init_layer_tile_map_0e_with_bounds_slot06_0e((Slot06Layer *)data_801aa5d4);
    init_layer_tile_map_0e_front_slot06_0e((Slot06Layer *)cam_obj);
    init_layer_tile_map_fields_b_slot06_0e((Slot06Layer *)data_801904d8);
}

void init_slot_tile_sprite_and_poly_buffers_gray_k_slot06_0e(void) {
    init_two_tile_prim_arrays_gray_slot06_0e();
    init_tile_prims_gray_two_sets_slot06_0e();
    init_two_banks_of_gray_32x32_sprite_prims_slot06_0e();
    init_poly_ft4_buffers_gray_slot06_0e();
}

void call_layer_handler_by_mode_slot06_0e(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_layer_position_and_advance_mode_on_flag_slot06_0e(layer);
    } else if (layer->mode == 1) {
        update_layer_by_state_with_parallax_slot06_0e(layer);
    }
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int init_layer_position_and_advance_mode_on_flag_slot06_0e(Slot06Layer *layer) {
    init_sprite_position((Sprite *)layer, 0x150, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->mode++;
        layer->field_8b = 0;
        if (game_state.count_index == 0) {
            layer->field_8b = 0xff;
        }
    }
}

void update_layer_by_state_with_parallax_slot06_0e(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->state_index == 0) {
        update_layer_start_or_scroll_down_slot06_0e(layer);
    } else if (layer->state_index == 1) {
        advance_layer_pos_y_to_limit_slot06_0e(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d += data_80190542;
    d /= 4;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 4;
    e += layer->field_0e;
    e += layer->pos_y_int;
    layer->field_26 = e;
}

void update_layer_start_or_scroll_down_slot06_0e(Slot06Layer *layer) {
    if (layer->field_00 == 2) {
        layer->field_00 = 1;
        layer->state_index++;
        layer->field_03 = 0;
        *(s32 *)&layer->pos_x = 0;
        *(s32 *)&layer->pos_y = 0xc00000;
        advance_layer_pos_y_to_limit_slot06_0e(layer);
    } else if (game_state.field_65 == 0 && layer->field_8b == 0) {
        *(s32 *)&layer->pos_y += 0x4000;
    }
}

void advance_layer_pos_y_to_limit_slot06_0e(Slot06Layer *layer) {
    s32 t;

    if (game_state.field_65 == 0) {
        t = *(s32 *)&layer->pos_y + 0x2000;
        *(s32 *)&layer->pos_y = t;
        if (t >= 0x100) {
            *(s32 *)&layer->pos_y = 0x1000000;
            layer->state_index++;
        }
    }
}

void init_first_layer_tile_map_slot06_0e(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb8ec_slot06_0e;
    layer->data_table = data_801eb8ec_slot06_0e;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
