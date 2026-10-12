/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801edc10_slot06_10[];
extern Slot06TileW data_801f50b0_slot06_10[2][0x5d];

void init_layer_cam_and_advance_mode_slot06_10(Slot06Layer *layer);
void update_layer_flags_and_parallax_position_slot06_10(Slot06Layer *layer);
void init_draw_mode_prim_4(Tx *tx);

void noop_empty_function_slot06_10(void) {
}

void noop_empty_function_2_slot06_10(void) {
}

void noop_byte_pointer_slot06_10(u8 *p) {
}

void noop_empty_function_3_slot06_10(void) {
}

void call_layer_handler_by_mode_slot06_10(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_layer_cam_and_advance_mode_slot06_10(layer);
    } else if (layer->mode == 1) {
        update_layer_flags_and_parallax_position_slot06_10(layer);
    }
}

void init_layer_cam_and_advance_mode_slot06_10(Slot06Layer *layer) {
    init_cam_position((Cam *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->countdown_timer = 8;
        layer->mode++;
    }
}

void update_layer_flags_and_parallax_position_slot06_10(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (game_state.field_65 == 0) {
        layer->countdown_timer--;
        if (layer->countdown_timer == 0) {
            layer->countdown_timer = 8;
            layer->flags_bits = (layer->flags_bits + 0x200) & 0x600;
        }
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d -= d / 8;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e += layer->field_0e;
    e += layer->pos_y_int;
    layer->field_26 = e;
}

void set_layer_tile_map_params_a_slot06_10(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801edc10_slot06_10;
    layer->data_table = data_801edc10_slot06_10;
    layer->packed_param = 0x6408;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_93_sprite_prims_slot06_10(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x5d; i++) {
            p = data_801f50b0_slot06_10[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
