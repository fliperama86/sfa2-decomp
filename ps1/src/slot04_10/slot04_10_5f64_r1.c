/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c6ce0_slot04_10[];
extern ObjectFnInt data_801c6ce8_slot04_10[];
extern u8 data_801c6d18_slot04_10[];
extern u8 data_801c6d80_slot04_10[];
extern SequenceStep *data_801c6dbc_slot04_10[];
extern ObjectFn data_801c6dc8_slot04_10[];
extern ObjectFn data_801c6dd8_slot04_10[];
extern ObjectFn data_801c6e9c_slot04_10[];
extern ObjectRef data_80190468;

int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void func_8011ffdc(Object *o);
void reset_and_push_block_object_type_8(Slab172 *o);
void load_velocities_from_table_slot04_10(Object *obj);
void load_velocities_from_table_2_slot04_10(Object *obj);
void set_vertical_motion_from_table_slot04_10(Object *obj);

void func_801b5f64_slot04_10(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) == 0) {
        update_sequence(obj);
    } else if ((t & 0xff) == 0xf) {
        obj->step_flags = t & 0xff00;
        data_80190468.p->vec_table_index = obj->direction_index * 4 + 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        play_side_sound_at(obj, obj->side, 6);
        if ((u8)apply_damage_to_other_return_defeated(obj, *(s16 *)((u8 *)data_801c6ce0_slot04_10 + (obj->direction_index & 0xfe)), 0) != 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            spawn_game_state_kind_object(obj);
        }
        play_sound_at(obj, obj->side, 0x319);
    } else {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 0x23, 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        obj->vel_x = 0x38000;
        obj->vel_y = 0x60000;
        obj->accel_y = -0x8000;
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void func_801b6148_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x2f);
    }
}

void func_801b61c4_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->facing_side = obj->facing_side ^ 1;
        load_velocities_from_table_2_slot04_10(obj);
    }
}

void update_sequence_or_finish_sequence_4_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        load_velocities_from_table_slot04_10(obj);
    }
}

void func_801b6270_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if ((t & 0xff) == 0xf) {
            apply_damage_to_other_return_defeated(obj, -0xed, 0);
            obj->step_flags = obj->step_flags & 0xff00;
            data_80190468.p->vec_table_index = obj->direction_index * 4 + 0x3c;
            play_sound_at(obj, obj->side, 0x319);
            func_80146960(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3 = 5;
        obj->active_state = 1;
        ((Slot04bObj *)obj)->field_298 = 0;
        ((Slot04bObj *)obj)->table_index_2 = 2;
        set_vertical_motion_from_table_slot04_10(obj);
        set_sequence_by_side_and_mode(obj, 0x2c);
    }
}

void func_801b6338_slot04_10(Object *obj) {
    data_801ad398 = data_801c6ce8_slot04_10[obj->field_15a](obj);
}

int has_remaining_count_slot04_10(Object *obj) {
    return obj->remaining_count != 0;
}

int func_801b638c_slot04_10(Object *obj) {
    return *(s16 *)&obj->meter >= 0x30;
}

int func_801b63a0_slot04_10(Object *obj) {
    return *(s16 *)&obj->meter >= 0x30;
}

int return_true_always_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_2_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_3_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_4_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_5_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_6_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_7_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_8_slot04_10(Object *obj) {
    return 1;
}

int return_true_always_9_slot04_10(Object *obj) {
    return 1;
}

void clear_seven_eight_byte_blocks_slot04_10(Object *obj) {
    s16 i;
    u16 j;

    for (i = 0; i < 7; i++) {
        ref_first.p = (Object *)((u8 *)obj + 0x2b0 + (i << 3));
        for (j = 0; j < 8; j++) {
            *(*(u8 **)&ref_first.p)++ = 0;
        }
    }
}

void call_handler_by_state_0_slot04_10(Object *obj) {
    data_801c6dc8_slot04_10[obj->state_0](obj);
}

void init_child_object_from_parent_tables_slot04_10(Object *obj) {
    u8 t;

    obj->variant_index_2 = 0xff;
    obj->state_0 = obj->state_0 + 1;
    obj->ot_depth = 0;
    ref_other.p = obj->parent;
    t = ref_other.p->field_49;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c6d80_slot04_10;
    ((Slot04bObj *)obj)->data_pointer = (s32)data_801c6d18_slot04_10;
    obj->active_state = 0;
    ((Slot04bObj *)obj)->field_5c = 0;
    obj->field_49 = t;
    set_sequence_entry(obj, data_801c6dbc_slot04_10[obj->vel_table_index >> 1]);
}

void call_handler_by_state_1_then_offset_slot04_10(Object *obj) {
    data_801c6dd8_slot04_10[obj->state_1](obj);
    func_8011ffdc(obj);
}

void func_801b6588_slot04_10(Object *obj) {
    ref_other.p = obj->parent;
    if (ref_other.p->frame->active == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void increment_state_0_slot04_10(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_record_slot04_10(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void call_handler_by_state_0_with_parent_slot04_10(Object *obj) {
    ref_other.p = obj->parent;
    data_801c6e9c_slot04_10[obj->state_0](obj);
}

void copy_data_pointers_from_ref_other_slot04_10(Object *obj) {
    obj->data_ptr_a = ref_other.p->data_ptr_a;
    obj->data_ptr_b = ref_other.p->data_ptr_b;
    obj->state_0 = obj->state_0 + 1;
    obj->field_1c = ref_other.p->field_1c;
    obj->table_index = ref_other.p->kind;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->sound_side = ref_other.p->sound_side;
    obj->variant_index = 0;
}
