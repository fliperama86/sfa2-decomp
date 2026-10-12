/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4c10_slot04_12[];
extern s32 data_801c4c28_slot04_12[];
extern u8 data_801c4c58_slot04_12[];
extern ObjectFn data_801c4c5c_slot04_12[];
extern u8 data_801c4c74_slot04_12[];
void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
int apply_facing_velocity_with_x_stop_slot04_12(Object *object);

void call_table_function_by_state_3_7_slot04_12(Object *obj) {
    data_801c4c10_slot04_12[obj->state_3](obj);
}

void start_shifted_motion_from_direction_table_slot04_12(Object *o) {
    o->state_3++;
    o->countdown = ((Slot04bObj *)o)->countdown + 0x400;
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    spawn_three_child_objects_at_object_spawn(o);
    if (o->facing_side != 0) {
        o->pos_x = o->pos_x + 0x18;
    } else {
        o->pos_x = o->pos_x - 0x18;
    }
    o->vel_x = data_801c4c28_slot04_12[o->direction_index * 2];
    o->accel_x = data_801c4c28_slot04_12[o->direction_index * 2 + 1];
    o->vel_y = data_801c4c28_slot04_12[o->direction_index * 2 + 2];
    o->accel_y = data_801c4c28_slot04_12[o->direction_index * 2 + 3];
    set_sequence_by_side_and_mode(o, o->direction_index + 0x53);
}

void spawn_children_when_countdown_high_byte_set_slot04_12(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if (o->countdown & 0xff00) {
        o->state_3++;
        o->countdown = ((Slot04bObj *)o)->countdown + 0x3800;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        play_side_sound_at(o, o->side, 6);
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0xc, 0x30);
    }
}

void count_down_then_set_table_value_sound_7_slot04_12(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if ((o->countdown & 0xff00) == 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            ((Slot04bObj *)o)->table_value = data_801c4c58_slot04_12[3];
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            ((Slot04bObj *)o)->table_value = data_801c4c58_slot04_12[o->direction_index >> 1];
        }
        o->field_165 = 0;
        play_side_sound_at(o, o->side, 7);
    }
}

void update_sequence_set_active_on_flag_slot04_12(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}

void update_fall_land_on_ground_slot04_12(Object *o) {
    if (apply_facing_velocity_with_x_stop_slot04_12(o) < 0 && ((s16)o->step_flags & 0x8000)) {
        o->accel_y = -0x6000;
        if (o->pos_y > o->ground_y) {
            o->pos_y_frac = 0;
            o->active_state = 0;
            o->active_flag_3 = 0;
            o->state_3++;
            o->pos_y = o->ground_y;
            play_object_sound_by_kind(o);
            set_sequence_by_side_and_mode(o, o->direction_index + 0x54);
            return;
        }
    }
    update_sequence(o);
}

void update_sequence_or_finish_set_other_code_slot04_12(Object *o) {
    if (((s16)o->step_flags & 0x8000) == 0) {
        update_sequence(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(o);
    }
}

void call_table_function_by_state_3_8_slot04_12(Object *obj) {
    data_801c4c5c_slot04_12[obj->state_3](obj);
}

void start_sequence_with_meter_and_children_slot04_12(Object *o) {
    o->state_3++;
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    spawn_three_child_objects_at_object_spawn(o);
    set_sequence_by_side_and_mode(o, 0x59);
}

void update_sequence_then_spawn_children_on_flag_byte_slot04_12(Object *o) {
    update_sequence(o);
    if (*(u8 *)&o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        play_side_sound_at(o, o->side, 8);
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0x1a, 0x33);
    }
}

void set_table_value_on_sequence_end_sound_9_slot04_12(Object *o) {
    u8 t;

    update_sequence(o);
    if (*(u8 *)&o->step_flags == 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            t = 3;
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            t = o->direction_index >> 1;
        }
        ((Slot04bObj *)o)->table_value = data_801c4c74_slot04_12[t];
        o->field_165 = 0;
        play_side_sound_at(o, o->side, 9);
    }
}

void update_sequence_or_spawn_pooled_object_slot04_12(Object *obj) {
    Object *p;
    s16 y;

    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x1d;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->scroll_factor = obj->scroll_factor;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->ground_y = obj->ground_y;
            p->parent = obj;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            play_side_sound_at(obj, obj->side, 0x11);
        }
        set_sequence_by_side_and_mode(obj, obj->direction_index + 0x5a);
    }
}
