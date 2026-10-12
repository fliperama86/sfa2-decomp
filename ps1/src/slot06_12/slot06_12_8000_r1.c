/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea52c_slot06_12[];
extern Slot06Tile data_801ef340_slot06_12[2][0x8c];
extern Slot06Tile data_801f6070_slot06_12[2][0x100];

void init_first_layer_tile_map_slot06_12(Slot06Layer *layer);
void set_layer_tile_map_and_rect_params_slot06_12(Slot06Layer *layer);
void set_layer_tile_map_params_b_slot06_12(Slot06Layer *layer);
void noop_layer_function_slot06_12(Slot06Layer *layer);
void scroll_layer_parallax_half_x_slot06_12(Slot06Layer *layer);
void init_tile_prims_gray_two_banks_slot06_12(void);
void init_tile_prims_gray_bank_slot06_12(void);
void init_two_buffers_of_32px_sprite_prims_slot06_12(void);
void noop_empty_function_2_slot06_12(void);
void draw_tile_layer_0_16px_slot06_12(void);
void draw_scrolling_tile_map_layer_slot06_12(void);
void draw_tilemap_layer_32px_9x13_slot06_12(void);
void noop_empty_function_slot06_12(void);

void draw_background_layers_by_flags_slot06_12(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_12();
    }
    if (*flags & 4) {
        draw_scrolling_tile_map_layer_slot06_12();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_12();
    }
    if (*flags & 4) {
        noop_empty_function_slot06_12();
    }
}

void init_four_layers_tile_maps_slot06_12(void) {
    init_first_layer_tile_map_slot06_12((Slot06Layer *)data_801aa544);
    set_layer_tile_map_and_rect_params_slot06_12((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_params_b_slot06_12((Slot06Layer *)cam_obj);
    noop_layer_function_slot06_12((Slot06Layer *)data_801904d8);
}

void init_gray_tile_banks_and_sprites_with_noop_b_slot06_12(void) {
    init_tile_prims_gray_two_banks_slot06_12();
    init_tile_prims_gray_bank_slot06_12();
    init_two_buffers_of_32px_sprite_prims_slot06_12();
    noop_empty_function_2_slot06_12();
}

void update_layer_by_mode_scroll_slot06_12(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_counted((Sprite *)layer, 0x40, -0x10);
    } else if (layer->mode == 1) {
        scroll_layer_parallax_half_x_slot06_12(layer);
    }
}

void scroll_layer_parallax_half_x_slot06_12(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x -= 0x7000;
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d /= 2;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d += d / 4;
    d += layer->field_0e;
    d += layer->pos_y_int;
    layer->field_26 = d;
}

void init_first_layer_tile_map_slot06_12(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea52c_slot06_12;
    layer->data_table = data_801ea52c_slot06_12;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_tile_prims_gray_two_banks_slot06_12(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Tile *t;
    Slot06Sprt16 *sp;
    Slot06Sprt16 *q;

    i = 0;
    do {
        j = 0;
        do {
            r = (Slot06Tile *)((u8 *)data_801ef340_slot06_12 + i * 0xf50 + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x8c);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f6070_slot06_12[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            init_draw_mode_prim_3((Tx *)(i * 0x1c00 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x1c00) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x100);
        i++;
    } while (i < 2);
}
