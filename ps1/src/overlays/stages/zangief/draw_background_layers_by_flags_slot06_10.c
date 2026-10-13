/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot06Tile data_801eec10_slot06_10[2][0x9f];
extern u16 data_801ebc10_slot06_10[];

void init_first_layer_tile_map_slot06_10(Slot06Layer *layer);
void init_layer_tile_map_fields_slot06_10(Slot06Layer *layer);
void set_layer_tile_map_params_a_slot06_10(Slot06Layer *layer);
void init_gray_prims_in_two_tables_slot06_10(void);
void init_two_banks_of_93_sprite_prims_slot06_10(void);
void draw_tile_layer_1_16px_slot06_10(void);
void draw_tilemap_layer_32px_9x13_slot06_10(void);
void noop_empty_function_2_slot06_10(void);
void noop_byte_pointer_slot06_10(u8 *p);
void noop_empty_function_3_slot06_10(void);
void init_tile_prim_rows_gray_draw_mode_b_slot06_10(void);
void draw_tile_layer_0_16px_slot06_10(void);
void init_sprite_at_1c0_zero(Sprite *sprite);

void draw_background_layers_by_flags_slot06_10(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_10();
    }
    if (*flags & 4) {
        draw_tile_layer_1_16px_slot06_10();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_10();
    }
    if (*flags & 4) {
        noop_empty_function_2_slot06_10();
    }
}

void init_background_layers_slot06_10(void) {
    init_first_layer_tile_map_slot06_10((Slot06Layer *)background_layers);
    init_layer_tile_map_fields_slot06_10((Slot06Layer *)main_layer);
    set_layer_tile_map_params_a_slot06_10((Slot06Layer *)cam_obj);
    noop_byte_pointer_slot06_10(fourth_layer);
}

void init_slot_tile_and_sprite_buffers_gray_m_slot06_10(void) {
    init_tile_prim_rows_gray_draw_mode_b_slot06_10();
    init_gray_prims_in_two_tables_slot06_10();
    init_two_banks_of_93_sprite_prims_slot06_10();
    noop_empty_function_3_slot06_10();
}

void update_layer_follow_with_damping_slot06_10(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_sprite_at_1c0_zero((Sprite *)layer);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)main_layer;
        d = l2->field_22;
        d -= l2->ref_x;
        d -= d / 16;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e -= e / 16;
        e += layer->field_0e;
        e += layer->pos_y_int;
        layer->field_26 = e;
    }
}

void init_first_layer_tile_map_slot06_10(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ebc10_slot06_10;
    layer->data_table = data_801ebc10_slot06_10;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_tile_prim_rows_gray_draw_mode_b_slot06_10(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eec10_slot06_10[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x9f);
        i++;
    } while (i < 2);
}
