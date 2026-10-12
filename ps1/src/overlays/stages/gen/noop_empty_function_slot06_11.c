/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ecdc0_slot06_11[];
extern Slot06TileW data_801f5600_slot06_11[2][0x30];

void init_draw_mode_prim_4(Tx *tx);

void noop_empty_function_slot06_11(void) {
}

void noop_empty_function_2_slot06_11(void) {
}

void noop_byte_pointer_slot06_11(u8 *p) {
}

void noop_empty_function_3_slot06_11(void) {
}

void update_layer_by_mode_follow_main_slot06_11(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_cam_position_and_consume_pending((Cam *)layer, 0x1c0, 0x10);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e += layer->field_0e;
        e += layer->pos_y_int;
        layer->field_26 = e;
    }
}

void init_layer_tile_map_fields_slot06_11(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ecdc0_slot06_11;
    layer->data_table = data_801ecdc0_slot06_11;
    layer->packed_param = 0x6418;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_48_sprite_prims_slot06_11(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x30; i++) {
            p = data_801f5600_slot06_11[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
