/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eaf18_slot06_02[];
extern Poly28 data_801f5cf8_slot06_02[2][0x84];

void func_8015c09c(void *prim);
void init_cam_object_then_advance_state_slot06_02(Object *obj);
void update_object_table_timer_and_parallax_slot06_02(Object *obj);
void set_sequence_index_from_table_slot06_02(Object *obj);

void set_layer_tile_map_and_scale_c_slot06_02(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eaf18_slot06_02;
    layer->data_table = data_801eaf18_slot06_02;
    layer->packed_param = 0x5816;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_poly_ft4_buffers_gray_slot06_02(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f5cf8_slot06_02[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0x84);
        i++;
    } while (i < 2);
}

void call_handler_by_state_0_two_states_slot06_02(Object *obj) {
    if (obj->state_0 == 0) {
        init_cam_object_then_advance_state_slot06_02(obj);
    } else if (obj->state_0 == 1) {
        update_object_table_timer_and_parallax_slot06_02(obj);
    }
}

void init_cam_object_then_advance_state_slot06_02(Object *obj) {
    init_cam_position((Cam *)obj, 0x40, 0);
    if (obj->field_01 != 0) {
        obj->field_01 = 0;
        ((Slot06Obj *)obj)->table_index = 0;
        obj->state_0++;
        set_sequence_index_from_table_slot06_02(obj);
    }
}
