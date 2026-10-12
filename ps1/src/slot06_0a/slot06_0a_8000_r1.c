/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e991c_slot06_0a[];

void init_first_layer_tile_map_slot06_0a(Slot06Layer *layer);
void init_layer_map_fields_stage_0a_a_slot06_0a(Slot06Layer *layer);
void init_layer_map_fields_stage_0a_b_slot06_0a(Slot06Layer *layer);
void noop_byte_pointer_slot06_0a(u8 *p);
void init_slot06_0a_two_tile_groups_gray_slot06_0a(void);
void init_slot06_0a_tiles_gray_slot06_0a(void);
void init_wide_tile_prim_buffers_32px_slot06_0a(void);
void noop_empty_function_3_slot06_0a(void);
void draw_tile_map_layer_16px_half_scroll_slot06_0a(void);
void draw_tile_layer_1_16px_slot06_0a(void);
void draw_tile_map_layer_32px_slot06_0a(void);
void noop_empty_function_2_slot06_0a(void);

void draw_background_layers_by_flags_slot06_0a(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_map_layer_16px_half_scroll_slot06_0a();
    }
    if (*flags & 4) {
        draw_tile_layer_1_16px_slot06_0a();
    }
    if (*flags & 8) {
        draw_tile_map_layer_32px_slot06_0a();
    }
    if (*flags & 4) {
        noop_empty_function_2_slot06_0a();
    }
}

void init_background_layers_slot06_0a(void) {
    init_first_layer_tile_map_slot06_0a((Slot06Layer *)data_801aa544);
    init_layer_map_fields_stage_0a_a_slot06_0a((Slot06Layer *)data_801aa5d4);
    init_layer_map_fields_stage_0a_b_slot06_0a((Slot06Layer *)cam_obj);
    noop_byte_pointer_slot06_0a(data_801904d8);
}

void func_801e80f0_slot06_0a(void) {
    init_slot06_0a_two_tile_groups_gray_slot06_0a();
    init_slot06_0a_tiles_gray_slot06_0a();
    init_wide_tile_prim_buffers_32px_slot06_0a();
    noop_empty_function_3_slot06_0a();
}

void update_layer_quarter_parallax_by_mode_slot06_0a(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_sprite_counted((Sprite *)layer, 0x40, 0);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
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
}

void init_first_layer_tile_map_slot06_0a(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e991c_slot06_0a;
    layer->data_table = data_801e991c_slot06_0a;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
