/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ce76c_slot04_17[];
extern Object *data_801ce778_slot04_17[];
extern ObjectFn data_801ce788_slot04_17[];
extern Slot04_17Rece790 data_801ce790_slot04_17[];
extern ObjectFn data_801ce844_slot04_17[];
extern ObjectFn data_801ce850_slot04_17[];
extern ObjectFn data_801ce858_slot04_17[];
extern u8 data_801ce860_slot04_17[];
extern ObjectFn data_801ce8f0_slot04_17[];
extern ObjectFn data_801ce900_slot04_17[];
extern Slot04_17Recec3c data_801cec3c_slot04_17[];

void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void set_position_from_other_box(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void set_sequence_and_tables_from_direction(Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void land_reset_fields_and_choose_next_state(Object *object);
void set_sequence_by_side(Object *object, int arg);
unsigned short get_pressed_low_byte(Object *object);
void update_sequence_with_input_check(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);
void add_to_meter_clamped(Object *object, short delta);

void call_reset_object_to_state_1_0_sequence_0_slot04_17(Object *obj);
Object *get_table_object_by_field_slot04_17(Object *obj);
int has_buttons_94_pressed_or_released_slot04_17(Object *obj);
int has_buttons_68_pressed_or_released_slot04_17(Object *obj);
void copy_kind_table_to_scratch_by_side_slot04_17(Object *obj);
void enable_active_flag_3_and_select_sequence_2_slot04_17(Object *obj);
void apply_step_flags_offset_to_pos_x_slot04_17(Object *obj);
int apply_velocity_and_acceleration_2_slot04_17(Object *obj);
void call_state_3_handler_wrapper_2_slot04_17(Object *obj);
void set_position_above_other_by_kind_slot04_17(Object *obj);
void reset_countdown_and_set_indicator_boxes_slot04_17(Object *obj);
void store_side_vram_row_to_buffer_slot04_17(Object *obj);
void call_side_routine_and_set_side_flag_slot04_17(Object *obj);
Object *get_table_object_by_field_2_slot04_17(Object *obj);
void queue_side_record_at_offset_position_slot04_17(Object *obj);

void update_sequence_or_load_velocity_table_2_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        call_reset_object_to_state_1_0_sequence_0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_14_slot04_17(Object *obj) {
    data_801ce76c_slot04_17[obj->state_3](obj);
}

void start_sequence_1b_set_facing_position_slot04_17(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 8);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (get_table_object_by_field_slot04_17(obj)->pos_x + 0xc0 >= obj->pos_x) {
            goto inc;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
inc:
        obj->facing_side++;
    }
    set_sequence_by_side_and_mode(obj, 0x1b);
    set_position_from_other_box(obj);
}

void update_sequence_with_hit_and_effect_flags_slot04_17(Object *obj) {
    if (obj->step_flags & 4) {
        obj->state_3++;
        set_other_hit_reaction_state_0(obj, 8, 5, 0xf, 0, 0, 1);
    } else if (obj->step_flags & 2) {
        obj->step_flags = obj->step_flags & 0xfffd;
        spawn_effect_at_object_offset(obj, obj->direction_index >> 1, -0x59, 0x1e);
        play_sound_at(obj, obj->side ^ 1, 0x30d);
        play_side_sound_at(obj, obj->side, 6);
    }
    update_sequence(obj);
    if (obj->step_flags & 1) {
        set_position_from_other_box(obj);
    }
}

void update_sequence_or_load_velocity_table_3_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        call_reset_object_to_state_1_0_sequence_0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void enable_active_flag_3_and_select_sequence_slot04_17(Object *obj) {
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

int apply_velocity_and_acceleration_slot04_17(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    return obj->vel_y += obj->accel_y;
}

int apply_facing_velocity_with_x_stop_slot04_17(Object *obj) {
    s32 a = obj->vel_x;

    if (obj->facing_side == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    return obj->vel_y = obj->vel_y + obj->accel_y;
}

int check_buttons_by_kind_and_flag_b_slot04_17(Object *obj) {
    if (obj->kind == 0x11) {
        if (obj->flags_28b & 1) {
            goto y;
        }
x:
        return has_buttons_68_pressed_or_released_slot04_17(obj);
    } else if (obj->flags_28b & 1) {
        goto x;
    }
y:
    return has_buttons_94_pressed_or_released_slot04_17(obj);
}

void call_reset_object_to_state_1_0_sequence_0_slot04_17(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void call_reset_object_to_state_1_0_0_2_sequence_1_slot04_17(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_state_3_handler_wrapper_slot04_17(Object *obj) {
    land_reset_fields_and_choose_next_state(obj);
}

int has_buttons_94_pressed_or_released_slot04_17(Object *obj) {
    return ((obj->pressed_bits | obj->released_bits) & 0x94) == 0x94;
}

int has_buttons_68_pressed_or_released_slot04_17(Object *obj) {
    return ((obj->pressed_bits | obj->released_bits) & 0x68) == 0x68;
}

Object *get_table_object_by_field_slot04_17(Object *obj) {
    return data_801ce778_slot04_17[obj->field_0e];
}

void call_handler_by_state_3_for_kinds_slot04_17(Object *obj) {
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        data_801ce788_slot04_17[obj->state_3](obj);
    }
}

void toggle_kind_bit_rebuild_tables_set_sequence_slot04_17(Object *obj) {
    obj->state_3++;
    obj->flags_28b = 0;
    obj->kind ^= 2;
    copy_kind_table_to_scratch_by_side_slot04_17(obj);
    select_box_tables(obj);
    build_metrics(obj);
    set_sequence_by_side(obj, 0x30);
}

void update_sequence_or_switch_on_input_slot04_17(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
        return;
    }
    if (obj->mode_flag_2 == 0) {
        if (get_pressed_low_byte(obj) & 0xffff) {
            set_state_1_0_5_and_read_input_direction(obj);
            return;
        }
        if (find_nearby_enemy_and_get_flag(obj)) {
            enter_state_2_6_by_button_input(obj);
            return;
        }
    }
    if ((u8)is_facing_differs_from_saved(obj)) {
        set_win_state_or_state_4_sequence(obj);
    } else {
        update_sequence(obj);
    }
}

void copy_kind_table_to_scratch_by_side_slot04_17(Object *obj) {
    u32 *dst;
    Slot04_17Rece790 *p;
    int k;
    int i;

    k = 2;
    if (obj->kind == 0x11) {
        k = 1;
    }
    dst = (u32 *)0x1f800100;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    for (i = 0; i < 15; i++) {
        p = &data_801ce790_slot04_17[i];
        dst[data_801ce790_slot04_17[i].a] = (&p->a)[k];
    }
}

void call_handler_by_input_dir_0_slot04_17(Object *obj) {
    data_801ce844_slot04_17[obj->input_dir_0 >> 1](obj);
}

void call_handler_by_state_3_clear_field_2_slot04_17(Object *obj) {
    obj->field_157 = 0;
    data_801ce850_slot04_17[obj->state_3](obj);
}

void start_state_by_direction_or_switch_state_slot04_17(Object *obj) {
    obj->state_3++;
    if (obj->direction_index == 0) {
        enable_active_flag_3_and_select_sequence_2_slot04_17(obj);
    } else if (obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) && (try_force_other_state_if_box_in_range(obj, -0x14, 0xe) & 0xff)) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        enable_active_flag_3_and_select_sequence_2_slot04_17(obj);
        if (obj->input_dir_second == 0 && obj->direction_index != 0) {
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
        }
        apply_step_flags_offset_to_pos_x_slot04_17(obj);
    }
}

void update_sequence_with_input_check_and_shift_x_slot04_17(Object *obj) {
    update_sequence_with_input_check(obj);
    apply_step_flags_offset_to_pos_x_slot04_17(obj);
}

void call_handler_by_state_3_set_flag_2_slot04_17(Object *obj) {
    obj->field_157 = 1;
    data_801ce858_slot04_17[obj->state_3](obj);
}

void advance_state_3_and_flag_on_direction_4_slot04_17(Object *obj) {
    obj->state_3++;
    enable_active_flag_3_and_select_sequence_2_slot04_17(obj);
    if (obj->input_dir_second != 0) {
        if (obj->direction_index == 4) {
            obj->field_278 = 1;
            apply_step_flags_offset_to_pos_x_slot04_17(obj);
        }
    } else if (obj->direction_index == 4) {
        obj->field_29c = 1;
        obj->sequence_hold_flag = 1;
    }
}

void update_sequence_with_input_check_and_shift_x_2_slot04_17(Object *obj) {
    update_sequence_with_input_check(obj);
    apply_step_flags_offset_to_pos_x_slot04_17(obj);
}

void update_airborne_land_or_change_sequence_slot04_17(Object *obj) {
    if (obj->active_flag != 0 && *(u8 *)&obj->step_flags != 0 && (obj->pressed_bits & 8)) {
        set_sequence_by_side_and_mode(obj, obj->variant_index != 0 ? 0x19 : 0x18);
    } else if (apply_velocity_and_acceleration_2_slot04_17(obj) < 0 && obj->pos_y >= obj->ground_y) {
        call_state_3_handler_wrapper_2_slot04_17(obj);
        play_object_sound_by_kind(obj);
    } else {
        update_sequence(obj);
    }
}

void set_sequence_from_direction_index_slot04_17(Object *obj) {
    s16 n;
    s16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, *(s16 *)((u8 *)data_801ce860_slot04_17 + (obj->direction_index & 0xfe)));
    n = 0xc;
    if (obj->variant_index != 0) {
        n = 0x12;
    }
    if (obj->input_dir_second != 0) {
        n += 3;
    }
    t = obj->direction_index >> 1;
    t += n;
    set_sequence_by_side_and_mode(obj, t);
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        obj->active_flag = 0;
    }
}

void enable_active_flag_3_and_select_sequence_2_slot04_17(Object *obj) {
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void apply_step_flags_offset_to_pos_x_slot04_17(Object *obj) {
    u16 t = obj->step_flags;
    s16 d = t & 0x7f;
    s16 e = d;

    if (d != 0) {
        obj->step_flags = t & 0xff80;
        if (obj->facing_side == 0) {
            e = -d;
        }
        obj->pos_x = e + obj->pos_x;
    }
}

int apply_velocity_and_acceleration_2_slot04_17(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    return obj->vel_y += obj->accel_y;
}

void call_reset_object_to_state_1_0_sequence_0_2_slot04_17(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void call_reset_object_to_state_1_0_0_2_sequence_1_2_slot04_17(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_state_3_handler_wrapper_2_slot04_17(Object *obj) {
    land_reset_fields_and_choose_next_state(obj);
}

void call_handler_by_state_0_with_other_slot04_17(Object *obj) {
    ref_other.p = obj->other;
    data_801ce8f0_slot04_17[obj->state_0](obj);
}

void init_indicator_object_above_other_slot04_17(Object *obj) {
    Object *o;

    obj->field_01 = 1;
    obj->data_ptr_a = spawned_object_data_a;
    obj->field_90 = (void *)0x800fb100;
    obj->state_0++;
    obj->data_ptr_b = spawned_object_data_b;
    set_position_above_other_by_kind_slot04_17(obj);
    o = ref_other.p->other;
    play_side_sound_at(o, o->side, 0xf);
    ((Slot04bObj *)obj)->field_47 = ref_other.p->field_28c;
    reset_countdown_and_set_indicator_boxes_slot04_17(obj);
    store_side_vram_row_to_buffer_slot04_17(obj);
}

void call_handler_by_state_1_or_end_state_0_slot04_17(Object *obj) {
    set_position_above_other_by_kind_slot04_17(obj);
    if ((*(u32 *)&ref_other.p->field_28c & 0x80ff) != 0x8000 && (game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) == 0) {
        data_801ce900_slot04_17[obj->state_1](obj);
        queue_side_record_at_offset_position_slot04_17(obj);
    } else {
        obj->state_0++;
        call_side_routine_and_set_side_flag_slot04_17(obj);
    }
}

void queue_side_record_at_offset_position_slot04_17(Object *obj) {
    Slot04_17Recec3c *r = &data_801cec3c_slot04_17[obj->other->side];
    Object *t = get_table_object_by_field_2_slot04_17(ref_other.p);

    r->c = obj->box_tables;
    r->x = obj->pos_x - t->pos_x - 8;
    r->y = obj->pos_y + t->pos_y;
    add_item_to_table((Object *)r);
}
