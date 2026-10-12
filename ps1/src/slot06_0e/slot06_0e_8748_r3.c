/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec8ec_slot06_0e[];
extern u16 data_801ed8ec_slot06_0e[];
extern u16 data_80190542;
extern Poly28 data_801f3df4_slot06_0e[2][0x84];
extern Slot06TileW data_801f6734_slot06_0e[2][0x75];
extern void (*data_801eac90_slot06_0e[])(Slot06Layer *layer);

int init_cam_and_advance_mode_when_flagged_slot06_0e(Slot06Layer *layer);
void update_layer_state_and_follow_main_layer_slot06_0e(Slot06Layer *layer);
void func_8015c09c(void *prim);
void init_draw_mode_prim_4(Tx *tx);

void init_layer_tile_map_fields_b_slot06_0e(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec8ec_slot06_0e;
    layer->data_table = data_801ec8ec_slot06_0e;
    layer->packed_param = 0x5810;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_poly_ft4_buffers_gray_slot06_0e(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f3df4_slot06_0e[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0x84);
        i++;
    } while (i < 2);
}

void call_layer_handler_by_mode_2_slot06_0e(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_cam_and_advance_mode_when_flagged_slot06_0e(layer);
    } else if (layer->mode == 1) {
        update_layer_state_and_follow_main_layer_slot06_0e(layer);
    }
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int init_cam_and_advance_mode_when_flagged_slot06_0e(Slot06Layer *layer) {
    init_cam_position((Cam *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->mode++;
        layer->field_8b = 0;
        if (game_state.count_index == 0) {
            layer->field_8b = 0xff;
        }
    }
}

void update_layer_state_and_follow_main_layer_slot06_0e(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    data_801eac90_slot06_0e[layer->state_index](layer);
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

void func_801e9554_slot06_0e(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        if (layer->field_8b == 0) {
            *(s32 *)&layer->pos_y += 0x8000;
            if ((s16)layer->pos_y_int >= 0x480) {
                layer->state_index++;
            }
        }
    }
}

void func_801e95b4_slot06_0e(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_y += 0x8000;
        if ((s16)layer->pos_y_int >= 0x700) {
            layer->state_index++;
            layer->countdown_timer = 0x140;
        }
    }
}

void count_down_layer_timer_then_signal_layer_slot06_0e(Slot06Layer *layer) {
    layer->field_8b = 0xff;
    layer->countdown_timer -= 1;
    if (layer->countdown_timer == 0) {
        layer->state_index++;
        ((Slot06Layer *)data_801aa544)->field_00 = 2;
        layer->field_8b = 0;
    }
}

void increment_layer_state_index_slot06_0e(Slot06Layer *layer) {
    layer->state_index++;
}

void func_801e9658_slot06_0e(Slot06Layer *layer) {
    layer->field_8b = 0xff;
}

void init_layer_tile_map_0e_front_slot06_0e(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ed8ec_slot06_0e;
    layer->data_table = data_801ed8ec_slot06_0e;
    layer->packed_param = 0x6008;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e9690_slot06_0e(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x75; i++) {
            p = data_801f6734_slot06_0e[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
