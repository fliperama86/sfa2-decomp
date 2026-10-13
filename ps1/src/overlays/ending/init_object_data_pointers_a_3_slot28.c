/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
extern void (*data_80030664_slot28[])(Object *);
void set_sequence_of_fourth_hud_object_3_slot28(Object *obj, int arg);
extern Object *data_800518fc_slot28[];

void init_object_data_pointers_a_3_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8002e1d4_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_8002e758_slot28;
}

void call_pass_handler_and_update_blocks_a_3_slot28(Object *obj) {
    data_80030664_slot28[screen_state->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void advance_pass_index_and_set_sequence_0_3_slot28(Object *obj) {
    HudState *h = screen_state;
    h->pass_index = h->pass_index + 1;
    set_sequence_of_fourth_hud_object_3_slot28(obj, 0);
}

void advance_pass_when_sequence_ends_a_3_slot28(Object *obj) {
    if (*(s16 *)&data_800518fc_slot28[3]->step_flags < 0) {
        HudState *h = screen_state;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
