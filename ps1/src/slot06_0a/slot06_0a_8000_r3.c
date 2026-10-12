/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec11c_slot06_0a[];
extern Slot06TileW data_801f3eb0_slot06_0a[2][0x3d];

void init_cam_position_with_fixed_origin(Cam *cam);
void init_draw_mode_prim_4(Tx *tx);
void toggle_flag_and_set_layer_scroll_half_slot06_0a(Slot06Layer *layer);

void noop_empty_function_slot06_0a(void) {
}

void noop_empty_function_2_slot06_0a(void) {
}

void noop_byte_pointer_slot06_0a(u8 *p) {
}

void noop_empty_function_3_slot06_0a(void) {
}

void call_handler_by_layer_mode_cam_or_follow_slot06_0a(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_cam_position_with_fixed_origin((Cam *)layer);
    } else if (layer->mode == 1) {
        toggle_flag_and_set_layer_scroll_half_slot06_0a(layer);
    }
}

void toggle_flag_and_set_layer_scroll_half_slot06_0a(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    layer->flags_bits ^= 0x200;
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
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

void init_layer_map_fields_stage_0a_b_slot06_0a(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec11c_slot06_0a;
    layer->data_table = data_801ec11c_slot06_0a;
    layer->packed_param = 0x6018;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_wide_tile_prim_buffers_32px_slot06_0a(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x3d; i++) {
            p = data_801f3eb0_slot06_0a[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
