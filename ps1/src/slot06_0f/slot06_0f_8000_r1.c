/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ef004_slot06_0f[2][0xc9];
extern u16 data_801ecbc4_slot06_0f[];
extern u16 data_80190542;

void init_first_layer_tile_map_slot06_0f(Slot06Layer *layer);
void func_801e8220_slot06_0f(void);
void init_layer_tile_map_0f_with_bounds_slot06_0f(Slot06Layer *layer);
void init_layer_tile_map_fields_slot06_0f(Slot06Layer *layer);
void init_layer_tile_map_params_short_slot06_0f(Slot06Layer *layer);
void init_two_tile_prim_tables_gray_slot06_0f(void);
void init_two_buffers_of_32px_sprite_prims_slot06_0f(void);
void init_poly_ft4_buffers_gray_slot06_0f(void);
void draw_tile_layer_0_16px_slot06_0f(void);
void draw_tile_layer_sprites_with_partial_row_slot06_0f(void);
void draw_tilemap_layer_32px_9x13_slot06_0f(void);
void draw_skewed_tile_layer_quads_slot06_0f(void);
void init_sprite_at_1c0_zero(Sprite *sprite);

void draw_background_layers_by_flags_slot06_0f(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_0f();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_0f();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_0f();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_0f();
    }
}

void init_four_layers_tile_maps_slot06_0f(void) {
    init_first_layer_tile_map_slot06_0f((Slot06Layer *)data_801aa544);
    init_layer_tile_map_0f_with_bounds_slot06_0f((Slot06Layer *)data_801aa5d4);
    init_layer_tile_map_fields_slot06_0f((Slot06Layer *)cam_obj);
    init_layer_tile_map_params_short_slot06_0f((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_0f(void) {
    func_801e8220_slot06_0f();
    init_two_tile_prim_tables_gray_slot06_0f();
    init_two_buffers_of_32px_sprite_prims_slot06_0f();
    init_poly_ft4_buffers_gray_slot06_0f();
}

void update_layer_by_mode_half_scroll_slot06_0f(Slot06Layer *layer) {
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

void init_first_layer_tile_map_slot06_0f(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ecbc4_slot06_0f;
    layer->data_table = data_801ecbc4_slot06_0f;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e8220_slot06_0f(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ef004_slot06_0f[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xc9);
        i++;
    } while (i < 2);
}
