/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801eb124_slot06_09[];
extern u16 data_801ec124_slot06_09[];
extern Poly28 data_801f34e4_slot06_09[2][0x58];
extern Slot06TileW data_801f5064_slot06_09[2][0x5b];
extern u16 data_80190542;

void func_8015c09c(void *prim);
void init_cam_position_with_fixed_origin(Cam *cam);
void init_draw_mode_prim_4(Tx *tx);

void init_layer_map_fields_basic_slot06_09(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb124_slot06_09;
    layer->data_table = data_801eb124_slot06_09;
    layer->packed_param = 0x5c02;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_poly_ft4_slot06_09(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f34e4_slot06_09[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0x58);
        i++;
    } while (i < 2);
}

void update_layer_by_mode_scroll_half_slot06_09(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->mode == 0) {
        init_cam_position_with_fixed_origin((Cam *)layer);
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

void set_layer_tile_map_and_scale_f_slot06_09(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec124_slot06_09;
    layer->data_table = data_801ec124_slot06_09;
    layer->packed_param = 0x6404;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_09(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x5b; i++) {
            p = data_801f5064_slot06_09[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
