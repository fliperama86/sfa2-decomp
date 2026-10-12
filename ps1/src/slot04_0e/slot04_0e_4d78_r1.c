/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_action_if_buttons_pressed(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);

void set_end_state_with_sequence_offset_slot04_0e(Object *obj, int a);
void check_input_set_state_flag_slot04_0e(Object *obj);
void apply_step_flags_byte_to_pos_x_slot04_0e(Object *obj);
void func_801b5088_slot04_0e(Object *obj);
u8 call_handler_by_limit_hi_returning_slot04_0e(Object *obj);

void update_timer_state_a_slot04_0e(Object *obj) {
    u8 t;

    t = ((Slot04bObj *)obj)->field_47 - 1;
    ((Slot04bObj *)obj)->field_47 = t;
    if (t & 0x80) {
        set_end_state_with_sequence_offset_slot04_0e(obj, 0);
    } else if ((((Slot04bObj *)obj)->mode_bits & 1) && ((Slot04bObj *)obj)->state_flag) {
        ((Slot04bObj *)obj)->field_47 = 0x3c;
        ((Slot04bObj *)obj)->state_flag = 0;
        obj->state_3++;
        func_801b5088_slot04_0e(obj);
        set_sequence_by_side_and_mode(obj, 0x4d);
    } else {
        check_input_set_state_flag_slot04_0e(obj);
        update_sequence(obj);
        apply_step_flags_byte_to_pos_x_slot04_0e(obj);
    }
}

void update_timer_state_b_slot04_0e(Object *obj) {
    u8 t;

    t = ((Slot04bObj *)obj)->field_47 - 1;
    ((Slot04bObj *)obj)->field_47 = t;
    if (t & 0x80) {
        set_end_state_with_sequence_offset_slot04_0e(obj, 1);
    } else {
        if ((((Slot04bObj *)obj)->mode_bits & 1) && ((Slot04bObj *)obj)->state_flag) {
            ((Slot04bObj *)obj)->field_47 = 0x28;
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x4e);
        } else {
            check_input_set_state_flag_slot04_0e(obj);
            update_sequence(obj);
        }
        apply_step_flags_byte_to_pos_x_slot04_0e(obj);
    }
}

void update_timer_state_c_slot04_0e(Object *obj) {
    u8 t;

    t = ((Slot04bObj *)obj)->field_47 - 1;
    ((Slot04bObj *)obj)->field_47 = t;
    if (t & 0x80) {
        set_end_state_with_sequence_offset_slot04_0e(obj, 2);
    } else {
        update_sequence(obj);
        apply_step_flags_byte_to_pos_x_slot04_0e(obj);
    }
}

void set_end_state_with_sequence_offset_slot04_0e(Object *obj, int a) {
    obj->state_3 = 4;
    obj->active_flag_4 = 0;
    set_sequence_by_side_and_mode(obj, (u16)(a + 0x4f));
    apply_step_flags_byte_to_pos_x_slot04_0e(obj);
}

void update_sequence_with_x_shift_or_end_move_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
        apply_step_flags_byte_to_pos_x_slot04_0e(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void check_input_set_state_flag_slot04_0e(Object *obj) {
    u8 r;

    if (obj->mode_flag_2 != 0) {
        r = call_handler_by_limit_hi_returning_slot04_0e(obj);
    } else {
        r = func_8013cac8(obj, 0, 0);
    }
    if (r != 0) {
        ((Slot04bObj *)obj)->state_flag = 0xff;
        func_801b5088_slot04_0e(obj);
    }
}
