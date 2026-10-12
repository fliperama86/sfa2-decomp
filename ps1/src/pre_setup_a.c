/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void enter_state_2_6_by_button_input(Object *object) {
    object->facing_side = object->saved_facing;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        ref_other.p = object->other;
        if (object->hit_count_2 != 0 && ref_other.p->field_157 != 0) {
            object->field_157 = 1;
            object->state_0 = 1;
            object->state_2 = 6;
            object->state_1 = 0;
            object->state_3 = 2;
            set_sequence_by_side(object, 0x18);
            return;
        }
    }
    if (object->action_timer == 0 && (object->input_bits_2 & 0x4000)) {
        object->field_157 = 1;
        object->state_0 = 1;
        object->state_2 = 6;
        object->state_1 = 0;
        object->state_3 = 2;
        set_sequence_by_side(object, 0x18);
    } else {
        object->state_0 = 1;
        object->field_157 = 0;
        object->state_1 = 0;
        object->state_2 = 6;
        object->state_3 = 0;
        set_sequence_by_side(object, 0x15);
    }
}

void set_win_or_lose_state_by_health(Object *object) {
    u8 v;
    s16 hp;
    int index;
    game_state.other_object = object->other;
    hp = object->health;
    if ((s16)game_state.other_object->health >= hp) {
        index = 0;
        object->state_0 = 1;
        object->state_1 = 5;
    } else {
        v = 4;
        if (hp == 0x90) {
            object->repeat_count = object->repeat_count + 1;
            v = 0x84;
        }
        index = 0;
        object->field_167 = v;
        *((u8 *)object + object->win_count + 0xcf) = v;
        object->state_0 = 1;
        object->state_1 = 4;
    }
    object->state_2 = 0;
    object->state_3 = 0;
    object->field_7e = 0;
    object->cursor_timer_b = 0;
    object->cursor_timer_a = 0;
    object->frame_count = 0;
    object->field_180 = 0;
    set_sequence_by_side(object, index);
}

void set_vel_x_from_motion_speed_table(Object *object) {
    s32 speed;
    int held = object->input_bits_2 & 0x8000;
    object->variant_index = held == 0;
    speed = motion_speed_table[object->kind * 2 + object->variant_index];
    if (object->facing_side != 0) speed = -speed;
    object->vel_x = speed;
}
