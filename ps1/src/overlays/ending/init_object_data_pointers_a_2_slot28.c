/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8002acc4_slot28[];
extern u8 data_8002b024_slot28[];
extern void (*data_8002c730_slot28[])(Object *);
void set_sequence_of_fourth_hud_object_2_slot28(Object *obj, int arg);

void init_object_data_pointers_a_2_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8002acc4_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_8002b024_slot28;
}

void call_pass_handler_and_update_blocks_a_2_slot28(Object *obj) {
    data_8002c730_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void advance_pass_index_and_set_sequence_0_2_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    set_sequence_of_fourth_hud_object_2_slot28(obj, 0);
}
