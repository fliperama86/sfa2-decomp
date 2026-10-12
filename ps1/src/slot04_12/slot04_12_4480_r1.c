/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4e64_slot04_12[];
extern s16 data_801c4e70_slot04_12[];

void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void func_80131638(Object *object);
int check_hit_other_active_state_nonzero(Object *object, s16 a, s16 b, s16 c, u16 d);
void advance_state_3_restore_facing_slot04_12(Object *obj);
void update_sequence_or_finish_on_flag_3_slot04_12(Object *obj);
void func_801b465c_slot04_12(Object *obj);
void func_801b4584_slot04_12(Object *obj);
void set_state_5_and_start_variant_sequence_slot04_12(Object *obj);
void init_jump_velocity_by_scroll_position_slot04_12(Object *obj);
void func_801b4ba8_slot04_12(Object *obj);
int apply_velocity_and_accel_return_vel_y_slot04_12(Object *obj);

void func_801b4480_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4) {
        play_side_sound_at(obj, obj->side, 0xb);
    }
    func_801b4ba8_slot04_12(obj);
}

void update_sequence_or_finish_on_flag_2_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4544_slot04_12(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b465c_slot04_12(obj);
    } else {
        func_801b4584_slot04_12(obj);
    }
}

void func_801b4584_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        advance_state_3_restore_facing_slot04_12(obj);
    } else {
        update_sequence_or_finish_on_flag_3_slot04_12(obj);
    }
}

void advance_state_3_restore_facing_slot04_12(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    func_801b4ba8_slot04_12(obj);
}

void update_sequence_or_finish_on_flag_3_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b465c_slot04_12(Object *obj) {
    data_801c4e64_slot04_12[obj->state_3](obj);
}

void func_801b469c_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4 && obj->attack_param_d != 0) {
        obj->active_flag_3 = 1;
        obj->state_3++;
        add_to_meter_clamped(obj, 2);
        obj->vel_y = 0x78000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = 0xfffe8000;
        }
        obj->active_state = 1;
        obj->field_157 = 0;
        obj->pos_y -= 0x10;
        set_sequence_by_side_and_mode(obj, 0x1b);
    } else {
        func_801b4ba8_slot04_12(obj);
    }
}

void update_sequence_or_finish_on_flag_4_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void update_airborne_motion_and_land_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (apply_velocity_and_accel_return_vel_y_slot04_12(obj) < 0 && obj->ground_y <= obj->pos_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->facing_side = obj->facing_side ^ 1;
        play_object_sound_by_kind(obj);
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void select_attack_sequence_by_direction_slot04_12(Object *obj) {
    int t = 0xc;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    if (obj->input_dir_second == 0) {
        if (obj->direction_index != 0) {
            if (obj->hit_flag_a != 0 && obj->ground_y - 0x30 < obj->pos_y) {
                if (check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                    obj->state_0 = 1;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    return;
                }
            }
        }
    } else if (obj->direction_index == 2 && obj->attack_param_d != 0) {
        set_state_5_and_start_variant_sequence_slot04_12(obj);
        return;
    }
    add_to_meter_clamped(obj, data_801c4e70_slot04_12[obj->direction_index >> 1]);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (u16)((obj->direction_index >> 1) + t));
}

void set_state_5_and_start_variant_sequence_slot04_12(Object *obj) {
    int t = 0x1c;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    add_to_meter_clamped(obj, 1);
    obj->step_limit = 0;
    obj->active_flag = 0;
    if (obj->variant_index != 0) {
        t = 0x1d;
    }
    set_sequence_by_side_and_mode(obj, t);
}

void update_airborne_motion_or_land_slot04_12(Object *obj) {
    if (obj->step_limit == 0) {
        if (obj->active_flag != 0) {
            init_jump_velocity_by_scroll_position_slot04_12(obj);
        } else if (apply_velocity_and_accel_return_vel_y_slot04_12(obj) < 0 && obj->pos_y >= obj->ground_y) {
            play_object_sound_by_kind(obj);
            func_80131638(obj);
        } else {
            update_sequence(obj);
        }
    }
}
