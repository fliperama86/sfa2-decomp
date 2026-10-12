/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c612c_slot04_0e[];
extern u8 data_801c6140_slot04_0e[];
extern ObjectFn data_801c6170_slot04_0e[];

void push_other_by_vel_x(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void start_action_if_buttons_pressed(Object *object);

void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
void set_variant_index_from_input_pair_slot04_0e(Object *obj);
u8 call_handler_by_limit_hi_returning_slot04_0e(Object *obj);
void call_handler_by_handler_index_if_active_slot04_0e(Slot04bObj *obj);
u8 get_pressed_bits_test_or_handler_result_slot04_0e(Object *obj);
void set_variant_velocity_start_sequence_slot04_0e(Object *obj);
void clear_active_flag_and_finish_state_slot04_0e(Object *obj);
void start_state_1_sequence_unless_paused_slot04_0e(Object *obj);

void save_counter_or_count_down_then_push_other_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->saved_count == 0) {
            obj->saved_count = obj->counter;
        }
    } else if (obj->field_47 != 0) {
        obj->field_47--;
    }
    push_other_by_vel_x(o);
}

void call_table_function_by_state_3_18_slot04_0e(Object *obj) {
    data_801c612c_slot04_0e[obj->state_3](obj);
}

void func_801b5828_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    call_handler_by_handler_index_if_active_slot04_0e((Slot04bObj *)o);
    o->accel_x = 0;
    o->vel_y = 0;
    o->accel_y = 0;
    obj->field_47 = 8;
    if (o->facing_side == 0) {
        o->vel_x = 0x50000;
    } else {
        o->vel_x = -0x50000;
    }
    a = 0x63;
    if (o->field_49 == 0) {
        a = 0x55;
    }
    a += o->direction_index >> 1;
    set_sequence_by_side_and_mode(o, a);
}

void update_move_or_start_input_sequence_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    BytePair t;
    u16 a;

    if (obj->mode == 0 && get_pressed_bits_test_or_handler_result_slot04_0e(o)) {
        t = get_direction_pair_from_pressed_bits(o);
        o->variant_index = t.first;
        o->state_3++;
        add_to_meter_clamped(o, 2);
        a = 0x66;
        if (o->field_49 == 0) {
            a = 0x22;
        }
        a += o->variant_index >> 1;
        set_sequence_by_side_and_mode(o, a);
    } else {
        apply_velocity_and_accel_to_position_slot04_0e(o);
        if ((s16)o->step_flags >= 0) {
            update_sequence(o);
        } else {
            ((Object *)o->other)->other_state_code = 5;
            o->active_flag_4 = 0;
            reset_object_to_state_1_0_0_2_sequence_1(o);
        }
    }
}

void launch_from_variant_table_then_update_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *p;

    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
        play_object_sound_by_kind(o);
        p = (s32 *)(data_801c6140_slot04_0e + ((o->variant_index << 3) & 0x3f8));
        o->vel_x = *p++;
        o->accel_x = *p++;
        o->vel_y = *p++;
        o->accel_y = *p;
        if (o->facing_side != 0) {
            o->vel_x = -o->vel_x;
            o->accel_x = -o->accel_x;
        }
    }
    update_sequence(o);
}

void move_land_start_sequence_thirty_seven_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    if (obj->ground_y >= obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x25);
    }
}

void update_sequence_or_end_move_a_2_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

u8 get_pressed_bits_test_or_handler_result_slot04_0e(Object *obj) {
    u8 r;

    if (obj->mode_flag_2 != 0) {
        r = call_handler_by_limit_hi_returning_slot04_0e(obj);
    } else {
        r = (obj->pressed_bits & 0x95) != 0;
    }
    return r;
}

void call_table_function_by_state_3_19_slot04_0e(Object *obj) {
    data_801c6170_slot04_0e[obj->state_3](obj);
}

void enter_next_state_meter_handler_set_sequence_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    obj->state_flag = 0;
    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    call_handler_by_handler_index_if_active_slot04_0e((Slot04bObj *)o);
    a = 0x69;
    if (o->field_49 == 0) {
        a = 0x26;
    }
    set_sequence_by_side_and_mode(o, a);
}

void launch_on_mode_set_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode != 0) {
        o->active_state = 1;
        o->vel_x = 0x50000;
        o->accel_x = -0x1800;
        o->accel_y = -0x5000;
        o->state_3++;
        o->vel_y = 0x48000;
        if (o->facing_side != 0) {
            o->vel_x = -o->vel_x;
            o->accel_x = -o->accel_x;
        }
    }
    update_sequence(o);
}

void move_until_landing_start_landing_sequence_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    if (obj->mode == 0 && obj->state_flag == 0) {
        set_variant_index_from_input_pair_slot04_0e(o);
    }
    apply_velocity_and_accel_to_position_slot04_0e(o);
    if (o->ground_y > o->pos_y) {
        update_sequence(o);
    } else {
        o->state_3++;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->active_flag = 0;
        o->pos_y = (u16)o->ground_y;
        play_object_sound_by_kind(o);
        a = 0x6a;
        if (o->field_49 == 0) {
            a = 0x21;
        }
        set_sequence_by_side_and_mode(o, a);
    }
}

void update_sequence_or_dispatch_by_state_flag_slot04_0e(Object *o) {
    s8 t;

    if (((Slot04bObj *)o)->mode == 0 && ((Slot04bObj *)o)->state_flag == 0) {
        set_variant_index_from_input_pair_slot04_0e(o);
    }
    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        t = ((Slot04bObj *)o)->state_flag;
        if (t == 0) {
            clear_active_flag_and_finish_state_slot04_0e(o);
        } else if (t & 0x80) {
            start_state_1_sequence_unless_paused_slot04_0e(o);
        } else {
            set_variant_velocity_start_sequence_slot04_0e(o);
        }
    }
}
