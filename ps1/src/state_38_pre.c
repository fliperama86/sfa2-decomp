/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void add_clamped_value(GameState *state, int delta) {
    data_80188ec4 += delta;
    clamp_meter_value_to_byte_range(state);
}

void clamp_meter_value_to_byte_range(GameState *state) {
    if ((s16)data_80188ec4 < 0) {
        data_80188ec4 = 0;
    } else if ((s16)data_80188ec4 >= 0x100) {
        data_80188ec4 = 0xff;
    }
}

void subtract_cost_and_add_random_delta_to_counter(GameState *state, Object *object) {
    int cost;

    if (state->field_0a == 0) {
        update_table_columns_unless_mode3_inactive(state);
        data_80188ecc = 1;
    }
    cost = 0x48;
    if (object->cost_index < 0x10) cost = table_80176d70[object->cost_index];
    data_80188ec4 = data_80188ec4 - cost;
    add_clamped_value(state, table_80176d80[get_random_byte() & 0x1f] - 0x18);
    clamp_meter_value_to_byte_range(state);
}

void update_ec4_from_round_time_and_health(GameState *state) {
    short a;
    Object *other;

    if (state->mode == 3 && state->mode3_active_flag == 0) return;
    if (state->round_time >= 0x50) {
        a = 0x10;
    } else if (state->round_time >= 0x3c) {
        a = 6;
    } else if (state->round_time >= 0x28) {
        a = 4;
    } else if (state->round_time >= 0x14) {
        a = 6;
    } else {
        a = 0xa;
    }
    add_clamped_value(state, a);
    other = state->focus_object;
    if (other != 0 && other->mode_flag_2 == 0) {
        if ((s16)other->health >= 0x70) {
            a = 0x10;
        } else if ((s16)other->health >= 0x50) {
            a = 0xa;
        } else if ((s16)other->health >= 0x30) {
            a = 8;
        } else {
            a = -((s16)other->health < 0x10) & 0xfff8;
        }
        add_clamped_value(state, a);
    }
    if (state->field_4d != 0) {
        a = 2;
        other = state->focus_object;
        if (other != 0 && other->mode_flag_2 != 0) a = -8;
        add_clamped_value(state, a);
    }
    adjust_counter_c_by_table_and_random(state);
    clamp_meter_value_to_byte_range(state);
    set_counter_shifted_from_counter_c(state);
}

void call_add_one_to_ec4_wrapper_2(GameState *state, Object *object) {
    add_one_to_ec4_if_object_unflagged(state, object);
}

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object) {
    if ((state->mode != 3 || state->mode3_active_flag != 0) && object->mode_flag_2 == 0)
        add_clamped_value(state, 1);
}

void set_flag_225_add_direction_to_counter(GameState *state, Object *object) {
    object->field_225 = 1;
    if ((state->mode != 3 || state->mode3_active_flag != 0) && object->mode_flag_2 == 0)
        add_clamped_value(state, object->direction_index + 2);
}

void add_three_to_ec4_if_object_unflagged(GameState *state, Object *object) {
    if ((state->mode != 3 || state->mode3_active_flag != 0) && object->mode_flag_2 == 0)
        add_clamped_value(state, 3);
}

void add_one_to_ec4_if_other_in_state(GameState *state, Object *object) {
    if ((state->mode != 3 || state->mode3_active_flag != 0) && object->mode_flag_2 == 0 &&
        object->round_count >= 2) {
        Object *other = object->other;
        if (*(u16 *)&other->state_0 == 2 && (u8)(other->state_2 - 5) < 2)
            add_clamped_value(state, 1);
    }
}
