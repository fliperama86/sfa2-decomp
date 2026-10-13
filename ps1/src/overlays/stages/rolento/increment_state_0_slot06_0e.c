/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801ef40c_slot06_0e[];
extern SequenceStep *data_801ef418_slot06_0e[];
extern u8 data_801ef4b4_slot06_0e[];
extern u16 data_801ef4bc_slot06_0e[];
extern ObjectFn data_801ef4c4_slot06_0e[];
extern u8 data_801ef4d4_slot06_0e[];
extern s16 data_801f8474_slot06_0e;
extern u8 data_801fb1f8_slot06_0e[];

int rand(void);
void start_sequence_from_lookup_index_slot06_0e(Object *obj);

void increment_state_0_slot06_0e(Object *obj) {
    obj->state_0++;
}

void release_object_record_4_slot06_0e(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void start_sequence_from_lookup_index_slot06_0e(Object *obj) {
    Object *c = (Object *)obj->linked_object_b;

    data_801f8474_slot06_0e = obj->table_index + obj->countdown;
    data_801f8474_slot06_0e = data_801fb1f8_slot06_0e[data_801f8474_slot06_0e];
    c->field_01 = 0;
    if (data_801f8474_slot06_0e == 3) {
        c->field_01 = 1;
    }
    obj->pos_x = data_801ef4bc_slot06_0e[data_801f8474_slot06_0e];
    data_801f8474_slot06_0e = data_801ef4b4_slot06_0e[data_801f8474_slot06_0e];
    set_sequence_entry(obj, data_801ef40c_slot06_0e[data_801f8474_slot06_0e]);
}

void call_handler_by_state_0_7_slot06_0e(Object *obj) {
    data_801ef4c4_slot06_0e[obj->state_0](obj);
}

void init_object_and_pick_random_8_byte_row_slot06_0e(Object *obj) {
    u32 i;
    int off;
    u8 *tbl;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->countdown = 0;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    set_sequence_entry(obj, data_801ef418_slot06_0e[0]);
    off = rand() & 0xf;
    off <<= 3;
    i = 0;
    tbl = data_801ef4d4_slot06_0e;
    for (; i < 8; i++) {
        data_801fb1f8_slot06_0e[i] = *(u8 *)(off + i + (u32)tbl);
    }
}

void update_object_sequence_until_scroll_y_slot06_0e(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_sequence_checked(obj);
    }
    if (((Slot06Layer *)cam_obj)->scroll_y >= 0x500) {
        if (((Slot06Layer *)background_layers)->state_index == 2) {
            obj->state_0++;
        }
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}

void increment_state_0_2_slot06_0e(Object *obj) {
    obj->state_0++;
}

void release_object_record_5_slot06_0e(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
