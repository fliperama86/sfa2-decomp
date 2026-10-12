/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80029ed8_slot28[];
extern Object *data_8005187c_slot28[];
void set_sequence_of_fourth_hud_object_slot28(Object *obj, int arg);

void advance_pass_and_show_first_object_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        Object *o;
        h->countdown_timer = 0x258;
        h->pass_index += 1;
        o = data_8005187c_slot28[0];
        o->pos_y = -0x50;
        o->field_01 = 1;
        set_sequence_from_table_5(o, 1, data_80029ed8_slot28);
        set_sequence_of_fourth_hud_object_slot28(obj, 5);
        start_fade_mode_1a01_and_wait();
    }
}

void start_countdown_when_sequence_flag_negative_slot28(Object *obj) {
    if (obj->field_f0 == 0 && (s16)data_8005187c_slot28[3]->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index += 1;
    }
}

void count_down_then_fade_and_advance_pass_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index += 1;
    }
}

void advance_substep_when_wait_flag_clear_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}
