/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801edd2c_slot06_12[];
extern Slot06TileW data_801f4cf0_slot06_12[2][0x4e];

void init_draw_mode_prim_4(Tx *tx);

void scroll_layer_parallax_quarter_x_slot06_12(Slot06Layer *layer);

void update_layer_wrap_offset_value_slot06_12(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    int pos;
    int t;

    l2 = (Slot06Layer *)data_801aa5d4;
    if (game_state.field_65 == 0) {
        layer->wrap_offset -= 0x4000;
    }
    layer->wrap_offset &= 0x3ffffff;
    d = ((u16 *)&layer->field_60)[1];
    d -= l2->ref_x;
    t = d >> 1;
    t += d >> 3;
    t += layer->wrap_offset >> 16;
    t += 0x80;
    t &= 0xff;
    t -= 0x80;
    layer->computed_value = t << 16;
}

void noop_empty_function_slot06_12(void) {
}

void noop_layer_function_slot06_12(Slot06Layer *layer) {
}

void noop_empty_function_2_slot06_12(void) {
}

void update_layer_by_mode_slot06_12(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_cam_position_and_consume_pending((Cam *)layer, 0x1c0, 0x10);
    } else if (layer->mode == 1) {
        scroll_layer_parallax_quarter_x_slot06_12(layer);
    }
}

void scroll_layer_parallax_quarter_x_slot06_12(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x -= 0x2000;
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d /= 4;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d /= 2;
    d += layer->field_0e;
    d += layer->pos_y_int;
    layer->field_26 = d;
}

void set_layer_tile_map_params_b_slot06_12(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801edd2c_slot06_12;
    layer->data_table = data_801edd2c_slot06_12;
    layer->packed_param = 0x5c08;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_buffers_of_32px_sprite_prims_slot06_12(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x4e; i++) {
            p = data_801f4cf0_slot06_12[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
