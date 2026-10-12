/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void update_saved_facing_for_both_players(void) {
    update_saved_facing_from_other_x(&player_left);
    update_saved_facing_from_other_x(&player_left + 1);
}

void update_saved_facing_from_other_x(Object *object) {
    s16 delta, magnitude, negative;
    if (object->field_7e == 0 && object->field_299 == 0 &&
        object->field_49 == 0) {
        negative = 0;
        delta = (u16)object->pos_x - (u16)object->other->pos_x;
        magnitude = delta;
        if (delta < 0) {
            negative = 1;
            magnitude = -delta;
        }
        if ((s16)(magnitude + 0x18) > 0x30) {
            object->saved_facing = 0;
            if (negative) object->saved_facing = 1;
        }
    }
}

void clear_flags_when_player_frames_inactive(void) {
    if (player_left.frame->active == 0) data_801985b8 = 0;
    if (player_right.frame->active == 0) data_80198225 = 0;
}

void set_box_distance_x_for_both_players(void) {
    set_box_distance_x(&player_left, &player_left + 1);
    set_box_distance_x(&player_left + 1, &player_left);
}

void set_box_distance_x(Object *a, Object *b) {
    int x, y;
    x = b->frame->box_index;
    y = 0;
    if (x != 0) {
        Box6 *box = &((Box6 *)b->unknown_148)[x];
        int extent = box->extent;
        x = box->origin;
        y = -extent;
        if (a->saved_facing != 1) {
            x = -x;
            y = extent;
        }
    }
    y = a->pos_x - (b->pos_x + x + y);
    if (y < 0) y = -y;
    a->compare_limit = y;
}

void update_both_players(void) {
    update_object_input_and_timers(&player_right);
    update_object_input_and_timers(&player_right - 1);
}

void update_player_left_object(void) {
    update_object_input_and_timers(&player_left);
}

void update_input_and_timers_for_both_players(void) {
    update_object_input_and_timers(&player_left);
    update_object_input_and_timers(&player_left + 1);
}

void update_player_right_object(void) {
    update_object_input_and_timers(&player_right);
}

void update_object_input_and_timers(Object *object) {
    if (object->other_state_code) object->other_state_code--;
    if (object->mode_flag_2 == 0) {
        if (game_state.versus_flag != 0 && data_801a6984[0] != 0 &&
            game_state.mode == object->side + 1)
            replay_input_log(object);
        load_remapped_input(object);
        update_input_bits(object);
    }
    if (object->countdown_timer_2) {
        object->countdown_timer_2--;
        if (object->countdown_timer_2 == 0) object->accum_count = 0;
    }
    call_state_0_handler_and_play_frame_sound(object);
}

void call_state_0_handler_and_play_frame_sound(Object *object) {
    handlers_04[object->state_0](object);
    play_frame_sound(object);
}

void noop_object_handler(Object *unused) {
}
