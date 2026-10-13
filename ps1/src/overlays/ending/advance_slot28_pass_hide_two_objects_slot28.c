/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ac4_slot28[];
extern ObjectRef data_80051ac8_slot28;
extern ObjectRef data_80051ad0_slot28;
void release_active_objects_in_table_8_slot28(void);
void set_sequence_of_fourth_hud_object_10_slot28(Object *obj, int arg);

void advance_slot28_pass_hide_two_objects_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        screen_state->pass_index++;
        release_active_objects_in_table_8_slot28();
        data_80051ac4_slot28[0]->field_01 = 0;
        data_80051ac8_slot28.p->field_01 = 0;
        set_sequence_of_fourth_hud_object_10_slot28(obj, 1);
        start_fade_mode_1a01_and_wait();
    }
}

void advance_pass_on_sequence_end_call_menu_6_slot28(Object *obj) {
    if ((s16)data_80051ad0_slot28.p->step_flags < 0) {
        screen_state->countdown_timer = 0xb4;
        screen_state->pass_index++;
        set_menu_command_by_code(6, 1);
    }
}

void advance_pass_on_countdown_negative_slot28(Object *obj) {
    HudState *h = screen_state;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->pass_index++;
        set_menu_command_by_code(1, 0x507);
        start_fade_mode_1801_and_wait();
    }
}
