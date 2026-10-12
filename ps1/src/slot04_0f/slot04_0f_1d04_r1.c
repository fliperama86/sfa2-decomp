/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5ac0_slot04_0f[];
extern ObjectFn data_801c5ad4_slot04_0f[];

void set_sequence_by_side(Object *object, int index);

void func_801b1fd8_slot04_0f(Object *obj);
void func_801b1ddc_slot04_0f(Object *obj);
void update_sequence_then_advance_state_3_2_slot04_0f(Object *obj);
void set_vel_x_toward_table_offset_target_slot04_0f(Object *o, int a, int pos);
void update_mode_change_select_row_slot04_0f(Object *obj);

void func_801b1d04_slot04_0f(Object *obj) {
    if (obj->field_49 != 0) {
        func_801b1fd8_slot04_0f(obj);
    } else {
        data_801c5ac0_slot04_0f[obj->state_3](obj);
    }
}

void func_801b1d64_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->state_3 = o->state_3 + 1;
    obj->set_flag = 1;
    add_to_meter_clamped(o, 2);
    *(s32 *)&obj->saved_pos_x_frac = *(s32 *)&o->other->pos_x_frac;
    obj->field_339 = 0xff;
    set_sequence_by_side_and_mode(o, 0x39);
    play_side_sound_at(o, o->side, 0xe);
    func_801b1ddc_slot04_0f(o);
}

/* The call of set_vel_x_toward_table_offset_target_slot04_0f passes one argument although the callee takes three: the original does not set the second and third argument registers before it. Written with the arguments (two zeros), this unit is 8 bytes longer and differs from the original in 10 instruction slots. */
void func_801b1ddc_slot04_0f(Object *o) {
    update_sequence(o);
    update_mode_change_select_row_slot04_0f(o);
    if ((s16)o->step_flags < 0) {
        o->state_3 = o->state_3 + 1;
        o->field_01 = 0;
        ((Slot04bObj *)o)->countdown = 0x10;
        ((void (*)(Object *))set_vel_x_toward_table_offset_target_slot04_0f)(o);
    }
}

void move_by_vel_x_until_countdown_ends_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown--;
    if (obj->countdown == 0) {
        o->field_01 = 1;
        o->state_3 = o->state_3 + 1;
        o->facing_side = o->saved_facing;
        set_sequence_by_side_and_mode(o, 0x3a);
        play_side_sound_at(o, o->side, 0xf);
    } else {
        *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    }
}

void update_sequence_then_advance_state_3_slot04_0f(Object *o) {
    update_sequence(o);
    update_mode_change_select_row_slot04_0f(o);
    if ((s16)o->step_flags < 0) {
        ((Slot04bObj *)o)->countdown = 1;
        o->state_3 = o->state_3 + 1;
        if (o->active_state == 0) {
            o->countdown = (o->countdown & 0xff00) | 6;
            set_sequence_by_side(o, 0);
        }
    }
}

void update_sequence_countdown_fall_or_reset_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    obj->countdown--;
    if (obj->countdown == 0) {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
            o->vel_y = 0;
            o->accel_x = 0;
            o->vel_x = 0;
            o->accel_y = -0x3000;
            set_sequence_by_side(o, 0x21);
        } else {
            reset_object_to_state_1_0_sequence_0(o);
        }
    }
}

void func_801b1fd8_slot04_0f(Object *obj) {
    data_801c5ad4_slot04_0f[obj->state_3](obj);
}

void start_sequence_55_saving_other_position_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 one = 1;

    o->state_3 = o->state_3 + 1;
    obj->set_flag = one;
    add_to_meter_clamped(o, 2);
    o->active_flag_4 = one;
    *(s32 *)&obj->saved_pos_x_frac = *(s32 *)&o->other->pos_x_frac;
    set_sequence_by_side_and_mode(o, 0x55);
    play_side_sound_at(o, o->side, 0xe);
    update_sequence_then_advance_state_3_2_slot04_0f(o);
}

/* The call of set_vel_x_toward_table_offset_target_slot04_0f passes one argument although the callee takes three: the original does not set the second and third argument registers before it. Written with the arguments (two zeros), this unit is 8 bytes longer and differs from the original in 6 instruction slots. */
void update_sequence_then_advance_state_3_2_slot04_0f(Object *o) {
    update_sequence(o);
    if ((s16)o->step_flags < 0) {
        ((Slot04bObj *)o)->countdown = 0x10;
        o->state_3 = o->state_3 + 1;
        ((void (*)(Object *))set_vel_x_toward_table_offset_target_slot04_0f)(o);
    }
}

void slide_by_vel_x_until_countdown_zero_b_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown--;
    if (obj->countdown == 0) {
        o->state_3 = o->state_3 + 1;
        set_sequence_by_side_and_mode(o, 0x56);
        play_side_sound_at(o, o->side, 0xf);
    } else {
        *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    }
}

void update_sequence_end_fall_or_reset_slot04_0f(Object *o) {
    update_sequence(o);
    if ((s16)o->step_flags < 0) {
        o->active_flag_4 = 0;
        if (o->active_state == 0) {
            reset_object_to_state_1_0_sequence_0(o);
        } else {
            o->state_0 = 1;
            o->state_2 = 3;
            o->state_3 = 3;
            o->state_1 = 0;
            o->vel_y = 0;
            o->accel_x = 0;
            o->vel_x = 0;
            o->accel_y = -0x6000;
            set_sequence_by_side(o, 0x21);
        }
    }
}
