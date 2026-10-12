/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eab68_slot06_0c[];
extern void (*data_801eab78_slot06_0c[])(Object *, int);
extern void (*data_801eab84_slot06_0c[])(Object *, u16);
extern u16 data_801eab98_slot06_0c[];
extern SequenceStep *data_801ebc1c_slot06_0c[];

void start_sequence_14_if_player_state_2_is_7_slot06_0c(Object *obj);
void start_sequence_15_if_player_state_2_is_8_slot06_0c(Object *obj);
int classify_object_x_against_players_slot06_0c(Object *obj);

void call_handler_by_state_0_2_slot06_0c(Object *obj) {
    data_801eab68_slot06_0c[obj->state_0](obj);
}

void init_object_start_sequence_5_slot06_0c(Object *obj) {
    obj->field_0f = 1;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    obj->state_0++;
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[5]);
}

void call_handler_by_state_1_with_player_zone_slot06_0c(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eab78_slot06_0c[obj->state_1](obj, (u16)classify_object_x_against_players_slot06_0c(obj));
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void update_facing_side_or_start_sequence_slot06_0c(Object *obj, int arg) {
    u16 a = arg;
    if (a != 0 && ((s16)obj->step_flags & 0x8000)) {
        if (a >= 3) {
            obj->facing_side = 0;
        } else {
            obj->facing_side = 1;
        }
        obj->state_1 = obj->facing_side + 1;
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[11]);
    } else {
        advance_sequence_checked(obj);
        if ((u8)obj->step_flags != 0) {
            obj->facing_side = obj->facing_side ^ 1;
            obj->step_timer = (get_random_byte() & 0x1f) + 8;
            obj->step_flags = obj->step_flags & 0xff00;
        }
    }
}

void call_handler_by_state_2_slot06_0c(Object *obj, int arg) {
    data_801eab84_slot06_0c[obj->state_2](obj, (u16)arg);
}

void update_state_2_handler_zone_3_or_4_slot06_0c(Object *obj, u16 arg) {
    u16 a = arg;
    int i;
    if (a == 3) {
        start_sequence_14_if_player_state_2_is_7_slot06_0c(obj);
        start_sequence_15_if_player_state_2_is_8_slot06_0c(obj);
        advance_sequence_checked(obj);
    } else {
        if (a == 4) {
            obj->state_2++;
            obj->vel_x = 0;
            i = 0xc;
        } else {
            obj->state_2 = 0;
            obj->state_1 = 0;
            i = 5;
        }
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
    }
}

void switch_sequence_by_vel_x_at_flag_byte_slot06_0c(Object *obj, u16 unused) {
    int i;
    int j;
    if (((s16)obj->step_flags & 0xff00) != 0) {
        if (obj->vel_x != 0) {
            i = 0xb;
            j = 0;
        } else {
            i = 0xd;
            j = 2;
        }
        obj->state_2 = j;
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
    } else {
        advance_sequence_checked(obj);
    }
}

void set_sequence_by_arg_3_or_4_else_reset_slot06_0c(Object *obj, u16 arg) {
    u16 a = arg;
    int i;
    if (a == 4) {
        advance_sequence_checked(obj);
    } else {
        if (a == 3) {
            i = 0xc;
            obj->state_2 = 1;
            obj->vel_x = 1;
        } else {
            obj->state_2 = 0;
            obj->state_1 = 0;
            i = 5;
        }
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
    }
}

void set_sequence_from_arg_table_at_flag_end_slot06_0c(Object *obj, u16 arg) {
    u16 *t;
    if (((s16)obj->step_flags & 0xff00) != 0) {
        obj->state_2++;
        t = data_801eab98_slot06_0c;
        obj->vel_x = t[(u16)arg];
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[t[(u16)arg]]);
    } else {
        advance_sequence_checked(obj);
    }
}
