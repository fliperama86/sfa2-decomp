/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801eb5b8_slot06_13[2][0xfa];
extern u16 data_801e8db8_slot06_13[];

void init_first_layer_tile_map_slot06_13(Slot06Layer *layer);
void init_layer_tile_map_fields_slot06_13(Slot06Layer *layer);
void set_layer_tile_map_params_slot06_13(Slot06Layer *layer);
void func_801e85c8_slot06_13(void);
void init_sprite_prims_32x32_slot06_13(void);
void draw_tile_layer_0_16px_slot06_13(void);
void draw_tile_layer_1_16px_slot06_13(void);
void draw_tilemap_layer_32px_9x13_slot06_13(void);
void noop_empty_function_2_slot06_13(void);
void noop_byte_pointer_slot06_13(u8 *p);
void noop_empty_function_3_slot06_13(void);
void func_801e8224_slot06_13(void);

void draw_background_layers_by_flags_slot06_13(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_13();
    }
    if (*flags & 4) {
        draw_tile_layer_1_16px_slot06_13();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_13();
    }
    if (*flags & 4) {
        noop_empty_function_2_slot06_13();
    }
}

void init_background_layers_slot06_13(void) {
    init_first_layer_tile_map_slot06_13((Slot06Layer *)data_801aa544);
    init_layer_tile_map_fields_slot06_13((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_params_slot06_13((Slot06Layer *)cam_obj);
    noop_byte_pointer_slot06_13(data_801904d8);
}

void func_801e80f0_slot06_13(void) {
    func_801e8224_slot06_13();
    func_801e85c8_slot06_13();
    init_sprite_prims_32x32_slot06_13();
    noop_empty_function_3_slot06_13();
}

void update_layer_parallax_eighth_slot06_13(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_sprite_counted((Sprite *)layer, 0x40, 0);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
        d /= 8;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e /= 8;
        e += layer->field_0e;
        e += layer->pos_y_int;
        layer->field_26 = e;
    }
}

void init_first_layer_tile_map_slot06_13(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e8db8_slot06_13;
    layer->data_table = data_801e8db8_slot06_13;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e8224_slot06_13(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eb5b8_slot06_13[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xfa);
        i++;
    } while (i < 2);
}
