/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Poly28 data_801f04d0_slot06_0b[2][0xb0];
extern Slot06TileW data_801f3bd0_slot06_0b[2][0x5b];
extern u16 data_801ebb7c_slot06_0b[];
extern u16 data_801eab7c_slot06_0b[];
extern u16 data_80190542;

void func_8015c09c(void *prim);
void init_draw_mode_prim_4(Tx *tx);

void set_layer_tile_map_param_580e_width_scale_slot06_0b(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eab7c_slot06_0b;
    layer->data_table = data_801eab7c_slot06_0b;
    layer->packed_param = 0x580e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_layer_poly_ft4_prims_slot06_0b(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f04d0_slot06_0b[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0xb0);
        i++;
    } while (i < 2);
}

void update_layer_position_half_offset_slot06_0b(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_cam_position_and_consume_pending((Cam *)layer, 0x140, 0);
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

void init_layer_tile_map_fields_slot06_0b(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ebb7c_slot06_0b;
    layer->data_table = data_801ebb7c_slot06_0b;
    layer->packed_param = 0x6004;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_0b(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x5b; i++) {
            p = data_801f3bd0_slot06_0b[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
