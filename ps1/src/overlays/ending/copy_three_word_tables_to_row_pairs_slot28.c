/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_8002a87c_slot28[];
extern u16 data_8002a07c_slot28[];
extern u16 data_8002a47c_slot28[];
extern u8 data_80028aa0_slot28[];
extern u8 data_80028e0c_slot28[];
extern void (*data_8002ac84_slot28[])(Object *);
void set_sequence_of_fourth_hud_object_slot28(Object *obj, int arg);

void copy_three_word_tables_to_row_pairs_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8002a87c_slot28[i];
        data_801a27e4_rows[5][i] = data_8002a87c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8002a07c_slot28[i];
        data_801a27e4_rows[7][i] = data_8002a07c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_8002a47c_slot28[i];
        data_801a27e4_rows[8][i] = data_8002a47c_slot28[i];
    }
}

void init_object_data_pointers_a_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_80028aa0_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_80028e0c_slot28;
}

void call_pass_handler_and_update_blocks_a_slot28(Object *obj) {
    data_8002ac84_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void advance_pass_index_and_set_sequence_0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    set_sequence_of_fourth_hud_object_slot28(obj, 0);
}
