/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ebc70_slot06_0c[];
extern Slot06Tile data_801ee070_slot06_0c[2][0x45];
extern u16 data_80190542;

void init_first_layer_tile_map_slot06_0c(Slot06Layer *layer);
void init_layer_tile_map_0c_with_bounds_slot06_0c(Slot06Layer *layer);
void set_layer_tile_map_param_6400_width_scale_slot06_0c(Slot06Layer *layer);
void init_layer_tile_map_fields_b_slot06_0c(Slot06Layer *layer);
void init_slot06_0c_tiles_gray_slot06_0c(void);
void init_slot06_0c_two_tile_groups_gray_slot06_0c(void);
void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_0c(void);
void init_two_banks_of_poly_ft4_slot06_0c(void);
void draw_tile_layer_0_16px_slot06_0c(void);
void draw_tile_layer_sprites_with_partial_row_slot06_0c(void);
void draw_tilemap_layer_32px_9x13_slot06_0c(void);
void draw_skewed_tile_layer_quads_slot06_0c(void);
void init_sprite_at_1c0_zero(Sprite *sprite);

void draw_background_layers_by_flags_slot06_0c(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_0c();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_0c();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_0c();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_0c();
    }
}

void init_four_layers_tile_maps_slot06_0c(void) {
    init_first_layer_tile_map_slot06_0c((Slot06Layer *)data_801aa544);
    init_layer_tile_map_0c_with_bounds_slot06_0c((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_param_6400_width_scale_slot06_0c((Slot06Layer *)cam_obj);
    init_layer_tile_map_fields_b_slot06_0c((Slot06Layer *)data_801904d8);
}

void init_slot_tile_groups_and_poly_buffers_gray_i_slot06_0c(void) {
    init_slot06_0c_tiles_gray_slot06_0c();
    init_slot06_0c_two_tile_groups_gray_slot06_0c();
    init_two_banks_of_91_gray_32x32_sprite_prims_slot06_0c();
    init_two_banks_of_poly_ft4_slot06_0c();
}

void update_layer_position_minus_eighth_slot06_0c(Slot06Layer *layer) {
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
        d -= d / 8;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e -= e / 8;
        e += layer->field_0e;
        e += layer->pos_y_int;
        layer->field_26 = e;
    }
}

void init_first_layer_tile_map_slot06_0c(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ebc70_slot06_0c;
    layer->data_table = data_801ebc70_slot06_0c;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_slot06_0c_tiles_gray_slot06_0c(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ee070_slot06_0c[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x45);
        i++;
    } while (i < 2);
}
