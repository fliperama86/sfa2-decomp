/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ec348_slot06_04[];
extern Poly28 data_801f4ae0_slot06_04[2][0x58];

void func_8015c09c(void *prim);
void init_layer_cam_then_advance_mode_slot06_04(Slot06Layer *layer);
void scroll_layer_left_and_update_offsets_slot06_04(Slot06Layer *layer);
void set_layer_flags_and_timer_from_table_slot06_04(Slot06Layer *layer);

void set_layer_tile_map_param_580e_width_scale_slot06_04(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec348_slot06_04;
    layer->data_table = data_801ec348_slot06_04;
    layer->packed_param = 0x580e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_poly_ft4_slot06_04(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f4ae0_slot06_04[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0x58);
        i++;
    } while (i < 2);
}

void call_layer_handler_by_mode_slot06_04(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_layer_cam_then_advance_mode_slot06_04(layer);
    } else if (layer->mode == 1) {
        scroll_layer_left_and_update_offsets_slot06_04(layer);
    }
}

void init_layer_cam_then_advance_mode_slot06_04(Slot06Layer *layer) {
    init_cam_position((Cam *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->table_index = 0;
        layer->mode++;
        set_layer_flags_and_timer_from_table_slot06_04(layer);
    }
}
