/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c74b8_slot04_11[];
extern u8 data_801c74c8_slot04_11[];
extern u8 data_801c74d0_slot04_11[];
extern ObjectFn data_801c74dc_slot04_11[];

void start_action_if_buttons_pressed(Object *object);
Block172 *pop_block_from_pool(void);
void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void apply_table_damage_to_other_spawn_child_slot04_11(Object *o);
void call_reset_object_to_state_1_0_sequence_0_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_slot04_11(Object *obj);
int apply_velocity_and_acceleration_slot04_11(Object *obj);

void update_air_land_or_active_flag_sequence_slot04_11(Object *obj) {
    s16 t;
    int a;
    u8 s;

    if (apply_velocity_and_acceleration_slot04_11(obj) < 0) {
        t = obj->ground_y;
        if (obj->pos_y >= t) {
            obj->pos_y = t;
            play_object_sound_by_kind(obj);
            a = 0x37;
            s = obj->variant_index;
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            obj->state_3 = 8;
            obj->active_flag_4 = 0;
            if (!(s & 0x80)) {
                a = (s & 0x7f) + 0x38;
            }
            set_sequence_by_side_and_mode(obj, a);
        }
    }
    if (obj->active_flag != 0) {
        a = obj->variant_index + 0x35;
        obj->active_flag = 0;
        obj->variant_index |= 0x80;
        set_sequence_by_side_and_mode(obj, a);
    } else {
        update_sequence(obj);
    }
}

void update_fall_land_with_sound_and_sequence_slot04_11(Object *obj) {
    if (apply_velocity_and_acceleration_slot04_11(obj) < 0 && obj->pos_y >= obj->ground_y) {
        play_object_sound_by_kind(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x37);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_set_other_state_code_5_3_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_0_2_sequence_1_slot04_11(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_5_slot04_11(Object *obj) {
    data_801c74b8_slot04_11[obj->state_3](obj);
}

void advance_state_and_set_sequence_by_half_direction_slot04_11(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x50);
}

void update_sequence_and_finish_on_flag_bit_slot04_11(Object *obj) {
    int v;

    update_sequence(obj);
    v = -1;
    if (obj->step_flags & 1) {
        obj->active_flag = 0;
        obj->step_limit = 0;
        obj->state_3++;
        if (obj->field_4b != 0) {
            v = 1;
        }
        obj->field_165 = v;
    }
}

void update_sequence_with_flag_spawn_and_lock_slot04_11(Object *o) {
    int k;
    u8 t;

    update_sequence(o);
    k = 6;
    if (!(o->step_flags & 1)) {
        o->state_3++;
        o->step_flags &= 0xfffe;
        if (o->field_4b == 0) {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            k = o->direction_index;
        }
        t = data_801c74c8_slot04_11[k & 0xfe];
        o->field_165 = 0;
        o->other_lock = t;
    }
    if (o->step_flags & 2) {
        o->step_flags &= 0xfffd;
        spawn_children_per_direction_index(o, -6, 0x4f);
        play_sound_at(o, o->side, 0x31c);
    }
}

void update_active_flag_hit_or_end_sequence_slot04_11(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_flag = 0;
    }
    if (obj->active_flag != 0) {
        obj->active_flag = 0;
        ref_other.p = obj->other;
        if (ref_other.p->hit_type != 0xff && (s16)ref_other.p->health >= 0) {
            apply_table_damage_to_other_spawn_child_slot04_11(obj);
        }
    }
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void apply_table_damage_to_other_spawn_child_slot04_11(Object *o) {
    Object *p;
    int i;
    u8 c;

    play_side_sound_at(o, o->side, 10);
    ref_other.p->damage_2 = 0;
    i = (u8)(o->direction_index << 1);
    c = data_801c74d0_slot04_11[i];
    ref_other.p->table_param = data_801c74d0_slot04_11[i + 2];
    ref_other.p->damage_2 = (ref_other.p->damage_2 & 0xff00) | data_801c74d0_slot04_11[i + 3];
    if (o->parent == 0) {
        ref_other.p->field_28c = c;
        ref_other.p->damage_countdown = 0x3c;
        ref_other.p->reference_count = 0x3c;
        p = (Object *)pop_block_from_pool();
        if (p == 0) {
            goto fail;
        }
        {
            p->active_count = 1;
            p->field_02 = 0x5e;
            p->other = o->other;
            p->parent = o;
            o->parent = p;
            p->field_0e = o->field_0e;
            p->ot_depth = 1;
            p->field_1c = o->field_1c;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = o->image_row;
            p->field_90 = o->field_90;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
            p->mode_id = 0x20;
            p->field_02 = 0x16;
            p->sound_side = o->sound_side;
        }
    } else {
        ref_other.p->reference_count += 0xf6;
        if (ref_other.p->reference_count == 0 || (ref_other.p->reference_count & 0x80)) {
        fail:
            ref_other.p->reference_count = 9;
        }
    }
}

void call_table_function_by_state_3_6_slot04_11(Object *obj) {
    data_801c74dc_slot04_11[obj->state_3](obj);
}

void advance_state_and_set_sequence_by_direction_slot04_11(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, obj->direction_index + 0x53);
}

void finish_sequence_restore_facing_side_slot04_11(Object *obj) {
    int v;

    update_sequence(obj);
    v = -1;
    if (obj->step_flags & 1) {
        obj->pos_x_frac = 0;
        obj->active_flag = 0;
        obj->step_limit = 0;
        obj->state_3++;
        obj->facing_side = obj->saved_facing;
        if (obj->field_4b != 0) {
            v = 1;
        }
        obj->field_165 = v;
    }
}
