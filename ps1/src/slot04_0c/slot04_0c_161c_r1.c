/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bd9d0_slot04_0c[];
extern ObjectFn data_801bda10_slot04_0c[];
extern ObjectFn data_801bda20_slot04_0c[];
extern s32 data_801bd9e0_slot04_0c[];
void start_action_if_buttons_pressed(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void func_801428e4(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);

void spawn_linked_pooled_object_with_sound_slot04_0c(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = 5;
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xc;
            p->table_index = 0;
            p->team_id = obj->team_id;
            p->sound_side = obj->sound_side;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x15);
        }
    }
}

void update_sequence_with_countdown_or_finish_slot04_0c(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t != 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(obj);
    tail:
        update_sequence(obj);
    }
}

void func_801b17d4_slot04_0c(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    }
}

void func_801b183c_slot04_0c(Object *obj) {
    data_801bd9d0_slot04_0c[obj->state_3](obj);
}

void func_801b187c_slot04_0c(Object *obj) {
    int n = 0x22;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        n = 0x39;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + n);
}

void set_velocities_from_table_on_step_flag_slot04_0c(Object *obj) {
    int i;
    s32 a;
    s32 b;

    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3++;
        obj->pos_y_frac = 0;
        i = obj->direction_index;
        a = data_801bd9e0_slot04_0c[i * 2];
        b = data_801bd9e0_slot04_0c[i * 2 + 1];
        obj->vel_y = data_801bd9e0_slot04_0c[i * 2 + 2];
        obj->accel_y = data_801bd9e0_slot04_0c[i * 2 + 3];
        if (obj->facing_side == 0) {
            obj->vel_x = -a;
            obj->accel_x = -b;
        } else {
            obj->vel_x = a;
            obj->accel_x = b;
        }
    }
    update_sequence(obj);
}

void func_801b19b8_slot04_0c(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        obj->field_157 = 1;
        set_sequence_by_side_and_mode(obj, 0x28);
    }
}

void update_sequence_or_reset_state_variant_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void func_801b1acc_slot04_0c(Object *obj) {
    data_801bda10_slot04_0c[obj->state_3](obj);
}

void func_801b1b0c_slot04_0c(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x29);
}

void func_801b1b60_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        spawn_children_per_direction_index(obj, 0x1e, 0x57);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void end_effect_set_other_lock_slot04_0c(Object *obj) {
    int s1 = 0;

    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            s1 = 4;
            obj->other->field_6b = 0xa;
        }
        obj->other_lock = s1;
    }
}

void spawn_linked_pooled_object_on_step_flag_slot04_0c(Object *obj) {
    Object *p;
    u16 t;
    s16 m;
    s16 y;

    update_sequence(obj);
    t = obj->step_flags;
    m = t & 0xff00;
    if (m < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else if (m == 0 && (t & 0xff) != 0) {
        obj->step_flags = 0;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xc;
            p->table_index = 2;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->sound_side = obj->sound_side;
            p->vel_table_index = obj->direction_index + 6;
            p->health = (obj->direction_index >> 1) + 2;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x15);
        }
    }
}

void func_801b1dd0_slot04_0c(Object *obj) {
    data_801bda20_slot04_0c[obj->state_3](obj);
}
