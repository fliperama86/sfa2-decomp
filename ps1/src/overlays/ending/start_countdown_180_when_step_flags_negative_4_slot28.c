/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_800519d0_slot28;
extern SequenceStep *data_800366e0_slot28[];
extern Object *data_800519c4_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern Object *data_80051998_slot28[];

void start_countdown_180_when_step_flags_negative_4_slot28(Object *obj) {
    if ((s16)data_800519d0_slot28.p->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
    }
}

void count_down_then_fade_and_advance_pass_4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index += 1;
    }
}

void advance_substep_when_wait_flag_clear_4_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}

void set_sequence_of_fourth_hud_object_6_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_800519c4_slot28[3], arg, data_800366e0_slot28);
}

void init_object_data_pointers_a_6_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_80035430_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_800357d8_slot28;
}

void clear_ten_entry_table_4_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051998_slot28[i] = 0;
    }
}
