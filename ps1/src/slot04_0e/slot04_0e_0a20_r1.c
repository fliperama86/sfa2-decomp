/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_by_side(Object *object, int arg);
void spawn_two_counted_objects_variant_6(Object *object);
void set_sequence_and_tables_from_direction(Object *object);
Block172 *pop_block_from_pool(void);
Pooled *alloc_pooled_b(void);
int match_pressed_direction_input(Object *object);
int try_set_direction_from_pressed_bits(Object *object);

void store_image_row_and_queue_job_slot04_0e(Object *obj);
void call_handler_by_input_dir_second_slot04_0e(Object *obj);
void call_handler_by_direction_index_slot04_0e(Object *obj);
void call_handler_by_direction_index_2_slot04_0e(Object *obj);
void func_801b16d0_slot04_0e(Object *obj);
void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
void copy_sixteen_table_entries_by_image_row_slot04_0e(Object *obj);
void load_velocities_from_table_slot04_0e(Object *obj);

extern u8 data_801a27e4[];
extern u16 data_801a2824[];
extern u16 data_801c5da4_slot04_0e[];
extern ObjectFn data_801c5efc_slot04_0e[];
extern ObjectFn data_801c5f0c_slot04_0e[];
extern ObjectFn data_801c5f28_slot04_0e[];
extern ObjectFn data_801c5f34_slot04_0e[];

void play_sound_on_mode_3_update_sequence_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 3) {
        obj->mode = 0;
        play_side_sound_at(o, o->field_02, 0x15);
    }
    update_sequence(o);
}


int apply_vel_y_to_other_position_slot04_0e(Object *a, Object *b) {
    *(s32 *)&b->pos_y_frac = *(s32 *)&b->pos_y_frac - a->vel_y;
    return a->vel_y += a->accel_y;
}

void set_sequence_by_index_per_side_slot04_0e(Object *object, int arg) {
    u16 index = arg;
    u16 fi;

    if (object->side == 0) {
        object->sequence = seqs_68_left[index];
    } else {
        object->sequence = seqs_118_right[index];
    }
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    fi = object->sequence->frame_index;
    object->sequence_changed_flag = 1;
    object->frame = object->frames + fi;
}

void advance_sequence_with_changed_flag_slot04_0e(Object *object) {
    Object *o = object;
    u16 fi;

    *(s16 *)&o->step_timer = (s16)o->step_timer - 1;
    if ((s16)o->step_timer == 0) {
        if ((s16)o->step_flags < 0) {
            o->sequence = o->sequence + o->sequence->loop_offset;
        } else {
            o->sequence = o->sequence + 1;
        }
        o->step_timer = o->sequence->duration;
        o->step_flags = o->sequence->flags;
        fi = o->sequence->frame_index;
        o->sequence_changed_flag = 1;
        o->frame = o->frames + fi;
    }
}

void func_801b0bb8_slot04_0e(Object *object) {
    copy_image_rows_and_load_image(object);
}

void store_image_row_and_queue_job_slot04_0e(Object *obj) {
    Rect r;
    Job *job;
    u8 *src;

    src = data_801a27e4 + (obj->image_row << 5);
    r.x = (obj->field_02 << 5) + 0x110;
    r.y = (game_state.frame_counter & 1) + 0x1e0;
    r.w = 0x10;
    r.h = 1;
    func_80158028(&r, src);
    job = (Job *)alloc_pooled_b();
    if (job) {
        r.x = 0x60;
        r.y = obj->image_row + 0x1e0;
        r.w = 0x10;
        r.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = r;
        job->src = src;
    }
}

void noop_empty_function_slot04_0e(void) {
}

void func_801b0cd0_slot04_0e(Object *obj) {
    u16 i;

    for (i = 0; i < 0x10; i++) {
        data_801a2824[(obj->image_row << 4) + i] = data_801c5da4_slot04_0e[i];
    }
    queue_image_upload_job(0, 6);
}

void call_handler_by_state_2_3_slot04_0e(Object *object) {
    data_801c5efc_slot04_0e[object->state_2](object);
}

void advance_state_2_and_restore_facing_slot04_0e(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->facing_side = object->saved_facing;
    set_sequence_by_side(object, 0);
}

void advance_state_2_when_timer_zero_slot04_0e(Object *object) {
    if (game_state.countdown_timer == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801b0df4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->state_2 = o->state_2 + 1;
    if (game_state_second.table_index == 0) {
        arg = 0x28;
    }
    set_sequence_by_side(o, arg);
}

void func_801b0e40_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 t;

    if (obj->field_47 != 0) {
        t = obj->field_47 - 1;
        obj->field_47 = t;
        if (t == 0) {
            game_state_second.side_bits = game_state.side_bits_2 | (1 << o->side);
        }
    }
    data_801c5f0c_slot04_0e[o->state_3](o);
    update_sequence(o);
}

void spawn_child_when_mode_is_one_type_seven_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    int x;

    if (obj->mode == 1) {
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x3b;
            p->table_index = 7;
            p->ot_depth = 0;
            p->field_02 = 0x14;
            p->team_id = o->team_id;
            p->parent = o;
            p->facing_side = o->facing_side;
            x = 0x33;
            if (o->facing_side == 0) {
                x = -0x33;
            }
            p->pos_x = x + o->pos_x;
            p->pos_y = obj->ground_y - 0x3b;
            obj->mode = 0;
            o->state_3 = o->state_3 + 1;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = o->image_row;
            p->mode_id = 0x20;
            p->field_90 = o->field_90;
            p->sound_side = o->sound_side;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
        }
    }
}

void advance_state_3_on_mode_2_play_sound_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 2) {
        obj->mode = 0;
        o->state_3 = o->state_3 + 1;
        play_side_sound_at(o, o->side, 0xc);
        copy_sixteen_table_entries_by_image_row_slot04_0e(o);
    }
}

void launch_upward_on_mode_3_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 3) {
        obj->mode = 0;
        o->vel_x = 0;
        o->accel_x = 0;
        o->state_3 = o->state_3 + 1;
        spawn_two_counted_objects_variant_6(o);
        play_sound_at(o, o->field_02, 0x321);
        o->vel_y = 0x5c000;
        o->accel_y = -0x5400;
        o->active_state = 1;
        set_sequence_by_side_and_mode_2(o, 0x13);
        store_image_row_and_queue_job_slot04_0e(o);
    }
}

void update_fall_until_ground_set_side_flag_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    apply_velocity_and_accel_to_position_slot04_0e(o);
    if (o->ground_y >= o->pos_y) {
        store_image_row_and_queue_job_slot04_0e(o);
    } else {
        obj->countdown = 4;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->state_3 = o->state_3 + 1;
        o->pos_y = obj->ground_y;
        set_sequence_by_side_and_mode_2(o, 0x14);
        set_side_flag(o);
        load_velocities_from_table_slot04_0e(o);
    }
}

void countdown_then_launch_up_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown--;
    if (obj->countdown == 0) {
        o->active_state = 1;
        o->vel_y = 0x28000;
        o->accel_y = -0x4800;
        o->state_3 = o->state_3 + 1;
        set_sequence_by_side_and_mode_2(o, 0x15);
    }
}

void land_on_ground_and_advance_state_3_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    apply_velocity_and_accel_to_position_slot04_0e(o);
    if (o->ground_y < o->pos_y) {
        o->state_3 = o->state_3 + 1;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->pos_y = obj->ground_y;
        set_sequence_by_side_and_mode_2(o, 0x16);
    }
}

void noop_slot00_slot04_0e(Object *obj) {
}

void call_handler_by_input_dir_0_nonzero_slot04_0e(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_801b16d0_slot04_0e(obj);
    } else {
        call_handler_by_input_dir_second_slot04_0e(obj);
    }
}

void call_handler_by_input_dir_second_slot04_0e(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_2_slot04_0e(obj);
    } else {
        call_handler_by_direction_index_slot04_0e(obj);
    }
}

void call_handler_by_direction_index_slot04_0e(Object *obj) {
    data_801c5f28_slot04_0e[obj->direction_index >> 1](obj);
}

void func_801b12f4_slot04_0e(Object *obj) {
    data_801c5f34_slot04_0e[obj->state_3](obj);
}

void check_range_or_start_direction_sequence_slot04_0e(Object *obj) {
    obj->state_3++;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x21, 7) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_reset_state_3_on_input_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->step_flags >= 0) {
        if ((obj->mode & 0x80) != 0 && (u8)match_pressed_direction_input(o) != 0 || (u8)try_set_direction_from_pressed_bits(o) != 0) {
            o->state_3 = 0;
        }
        update_sequence(o);
    } else {
        reset_object_to_state_1_0_sequence_0(o);
    }
}
