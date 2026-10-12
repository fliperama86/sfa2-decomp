/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ed5e4_slot06_0d[2][0x74];
extern u16 data_801eb1e4_slot06_0d[];
extern u16 data_80190542;

void init_first_layer_tile_map_slot06_0d(Slot06Layer *layer);
void init_layer_tile_map_0d_with_bounds_slot06_0d(Slot06Layer *layer);
void set_layer_tile_map_param_6414_width_scale_slot06_0d(Slot06Layer *layer);
void init_layer_tile_map_fields_slot06_0d(Slot06Layer *layer);
void init_two_tile_prim_tables_gray_slot06_0d(void);
void init_sprite_prims_32x32_gray_slot06_0d(void);
void init_two_banks_of_poly_ft4_slot06_0d(void);
void init_two_rows_of_116_gray_tile_prims_slot06_0d(void);
void draw_tile_map_layer_16px_half_scroll_slot06_0d(void);
void draw_tile_layer_sprites_with_partial_row_slot06_0d(void);
void draw_skewed_tile_layer_quads_slot06_0d(void);
void draw_tile_map_layer_32px_slot06_0d(void);
void init_sprite_once_with_mode_y(Sprite *sprite);

void draw_background_layers_by_flags_slot06_0d(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_map_layer_16px_half_scroll_slot06_0d();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_0d();
    }
    if (*flags & 8) {
        draw_tile_map_layer_32px_slot06_0d();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_0d();
    }
}

void init_four_layers_tile_maps_slot06_0d(void) {
    init_first_layer_tile_map_slot06_0d((Slot06Layer *)data_801aa544);
    init_layer_tile_map_0d_with_bounds_slot06_0d((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_param_6414_width_scale_slot06_0d((Slot06Layer *)cam_obj);
    init_layer_tile_map_fields_slot06_0d((Slot06Layer *)data_801904d8);
}

void init_slot_tile_sprite_and_poly_buffers_gray_j_slot06_0d(void) {
    init_two_rows_of_116_gray_tile_prims_slot06_0d();
    init_two_tile_prim_tables_gray_slot06_0d();
    init_sprite_prims_32x32_gray_slot06_0d();
    init_two_banks_of_poly_ft4_slot06_0d();
}

void update_layer_by_mode_half_scroll_slot06_0d(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_sprite_once_with_mode_y((Sprite *)layer);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
        d += data_80190542;
        d /= 2;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e /= 2;
        e += layer->field_0e;
        e += layer->pos_y_int;
        layer->field_26 = e;
    }
}

void init_first_layer_tile_map_slot06_0d(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb1e4_slot06_0d;
    layer->data_table = data_801eb1e4_slot06_0d;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_rows_of_116_gray_tile_prims_slot06_0d(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ed5e4_slot06_0d[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x74);
        i++;
    } while (i < 2);
}
