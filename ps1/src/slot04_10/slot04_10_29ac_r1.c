/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6a2c_slot04_10[];
extern ObjectFn data_801c6a34_slot04_10[];
extern ObjectFn data_801c6a44_slot04_10[];
extern ObjectRef data_80190468;

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void load_velocities_from_table_slot04_10(Object *obj);
void choose_random_action_values_slot04_10(Object *obj, int arg);
void update_walk_by_input_or_random_slot04_10(Object *obj);

void update_shift_x_or_finish_to_other_state_slot04_10(Object *obj) {
    s16 t;
    int a;

    update_sequence(obj);
    t = obj->step_flags;
    if (t >= 0) {
        if ((u8)t == 3) {
            a = 0x30;
            obj->step_flags = t & 0xff00;
            if (obj->facing_side == 0) {
                a = -0x30;
            }
            obj->pos_x = a + obj->pos_x;
        }
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_10(obj);
    }
}

void call_table_function_by_state_3_10_slot04_10(Object *obj) {
    data_801c6a2c_slot04_10[obj->state_3](obj);
}

void advance_state_3_on_step_flag_then_update_2_slot04_10(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}

void update_sequence_or_set_other_code_5_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_10(obj);
    }
}

void call_table_function_by_state_3_11_slot04_10(Object *obj) {
    data_801c6a34_slot04_10[obj->state_3](obj);
}

void start_backward_move_sequence_slow_slot04_10(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    ((Slot04bObj *)obj)->field_298 = 4;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged((GameState *)data_80190468.p, obj);
    obj->vel_x = 0xfffe8000;
    a = 0x47;
    if (obj->field_49 != 0) {
        a = 0x51;
    }
    set_sequence_by_side_and_mode(obj, a);
    if (obj->mode_flag_2 != 0) {
        choose_random_action_values_slot04_10(obj, 0x68);
    }
}

void call_table_function_by_state_3_12_slot04_10(Object *obj) {
    data_801c6a44_slot04_10[obj->state_3](obj);
}

void start_backward_move_sequence_fast_slot04_10(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    ((Slot04bObj *)obj)->field_298 = 4;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged((GameState *)data_80190468.p, obj);
    a = 0x48;
    obj->vel_x = 0xffff0000;
    if (obj->field_49 != 0) {
        a = 0x52;
    }
    set_sequence_by_side_and_mode(obj, a);
    if (obj->mode_flag_2 != 0) {
        choose_random_action_values_slot04_10(obj, 0x98);
    }
}

void choose_random_action_values_slot04_10(Object *obj, int arg) {
    u8 a[4] = { 1, 1, 0, 0xff };
    u8 b[4] = { 0, 4, 8, 0x10 };
    u8 c[4] = { 8, 0xc, 0x10, 0x20 };

    ((Slot04bObj *)obj)->random_value_a = 0;
    ((Slot04bObj *)obj)->field_1aa = 0;
    ((Slot04bObj *)obj)->random_countdown = 0;
    if (get_random_byte() & 3) {
        if (obj->other->link_flags != 0) {
            ((Slot04bObj *)obj)->random_value_a = 1;
            ((Slot04bObj *)obj)->field_1aa = 0;
            ((Slot04bObj *)obj)->random_countdown = 0x46;
        } else if ((s16)obj->compare_limit <= (s16)arg) {
            ((Slot04bObj *)obj)->random_value_a = 1;
            ((Slot04bObj *)obj)->field_1aa = 0;
            ((Slot04bObj *)obj)->random_countdown = 0x46;
        } else {
            ((Slot04bObj *)obj)->random_value_a = a[get_random_byte() & 3];
            ((Slot04bObj *)obj)->field_1aa = b[get_random_byte() & 3];
            ((Slot04bObj *)obj)->random_countdown = c[get_random_byte() & 3];
        }
    }
}

void advance_state_3_on_step_flag_byte_slot04_10(Object *obj) {
    u16 t = obj->step_flags;

    if ((u8)t == 0) {
        update_sequence(obj);
    } else {
        obj->step_flags = t & 0xff00;
        obj->state_3 = obj->state_3 + 1;
        update_walk_by_input_or_random_slot04_10(obj);
    }
}
