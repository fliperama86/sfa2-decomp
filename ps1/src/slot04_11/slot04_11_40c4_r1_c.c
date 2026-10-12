/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7798_slot04_11[];
extern u8 data_801c77a8_slot04_11[];
extern ObjectFn data_801c77b8_slot04_11[];

void call_reset_object_to_state_1_0_sequence_0_slot04_11(Object *obj);
void call_handler_by_state_3_for_kinds_slot04_11(Object *obj);
u8 mirror_value_by_facing_side_slot04_11(Object *obj, int a);
u8 get_random_table_byte_slot04_11(Object *obj);
int apply_velocity_and_acceleration_slot04_11(Object *obj);
void latch_pressed_bits_into_input_bits_slot04_11(Object *obj);

void update_land_check_step_count_sequence_slot04_11(Object *obj) {
    Config *c;

    latch_pressed_bits_into_input_bits_slot04_11(obj);
    apply_velocity_and_acceleration_slot04_11(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        if (obj->step_count_2 < obj->step_limit && ((c = game_state.config)->mode_flag_b | c->mode_flag_c | c->mode_flag_a) == 0 && is_other_object_state_0_and_1_six_one(obj) == 0) {
            obj->state_3 = 4;
            set_sequence_by_side_and_mode(obj, 0x4c);
        } else {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x6f);
        }
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_end_with_other_code_5_2_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void latch_pressed_bits_into_input_bits_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->mode_flag_2 == 0) {
        if (*(u16 *)&obj->input_bits == 0) {
            if (o->pressed_bits != 0) {
                *(u16 *)&obj->input_bits = o->pressed_bits & 0x68;
            }
        }
    }
}

u8 mirror_value_by_facing_side_slot04_11(Object *obj, int a) {
    if (obj->facing_side != 0) {
        return data_801c7798_slot04_11[a >> 3];
    }
    return a;
}

u8 get_random_table_byte_slot04_11(Object *obj) {
    return data_801c77a8_slot04_11[get_random_byte() & 0xf];
}

void call_state_3_handler_for_two_kinds_slot04_11(Object *obj) {
    call_handler_by_state_3_for_kinds_slot04_11(obj);
}

void call_table_function_by_state_3_9_slot04_11(Object *obj) {
    data_801c77b8_slot04_11[obj->state_3](obj);
}

void start_state_decrement_remaining_count_slot04_11(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    set_sequence_by_side_and_mode(obj, 0x20);
}
