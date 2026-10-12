/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801eb8dc_slot06_0e[];

void init_object_params_and_four_sequence_cursors_slot06_0e(Object *obj);
void call_handler_by_state_1_0_or_1_slot06_0e(Object *obj);
int update_cursors_by_scroll_x_range_slot06_0e(Object *obj);
void noop_slot00_slot06_0e(Object *obj);
void load_palette_record_by_index_slot06_0e(Slot06Pal *pal);

void call_handler_by_state_0_slot06_0e(Object *obj) {
    data_801eb8dc_slot06_0e[obj->state_0](obj);
}

void init_object_params_and_four_sequence_cursors_slot06_0e(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    Slot06Pal *p0 = (Slot06Pal *)((u8 *)obj + 0x30);
    Slot06Pal *p1 = (Slot06Pal *)((u8 *)obj + 0x40);
    Slot06Pal *p2 = (Slot06Pal *)((u8 *)obj + 0x50);
    Slot06Pal *p3 = (Slot06Pal *)((u8 *)obj + 0x6c);
    obj->init_param_b = 3;
    obj->table_base_index = 4;
    obj->init_param_e = 5;
    obj->init_param_j = 6;
    obj->init_param_a = 0x150;
    obj->sequence_index = 0x1a0;
    obj->size_param = 0x330;
    obj->init_param_h = 0x370;
    obj->field_01 = 0;
    obj->offset_b = 0x50;
    obj->init_param_c = 0;
    obj->init_param_d = 0;
    obj->init_param_i = 0x50;
    obj->step_count = obj->step_count + 1;
    load_palette_record_by_index_slot06_0e(p0);
    load_palette_record_by_index_slot06_0e(p1);
    load_palette_record_by_index_slot06_0e(p2);
    load_palette_record_by_index_slot06_0e(p3);
}

void call_handler_by_state_1_0_or_1_slot06_0e(Object *obj) {
    if (obj->state_1 == 0) {
        update_cursors_by_scroll_x_range_slot06_0e(obj);
    } else if (obj->state_1 == 1) {
        noop_slot00_slot06_0e(obj);
    }
}
