/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_800797e4_slot00[];
void select_box_tables_slot00_slot00(Object *obj);

void move_by_velocity_and_advance_sequence_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;

    select_box_tables_slot00_slot00(o);
    obj->pos_x_hi += o->vel_x;
    obj->pos_y_hi -= o->vel_y;
    advance_sequence_checked(o);
    set_state_0_if_x_outside_margin_narrow(o);
}

void call_handler_by_state_2_unless_paused_b_slot00(Object *o) {
    Config *config = game_state.config;

    if ((config->pause_flag_b | config->pause_flag_a) == 0) {
        data_800797e4_slot00[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void advance_state_2_set_countdown_2_slot00(Object *o) {
    o->countdown = 2;
    o->state_2++;
    set_sequence_by_sound_side(o, o->active_flag_2);
}

void count_down_then_reset_states_1_0_1_0_slot00(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown == 0) {
        o->active_count = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 1;
        o->state_3 = 0;
        select_box_tables_slot00_slot00(o);
    }
    advance_sequence_checked(o);
}
