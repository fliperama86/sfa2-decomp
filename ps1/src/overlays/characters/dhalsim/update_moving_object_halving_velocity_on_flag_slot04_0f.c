/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c5e6c_slot04_0f[];
extern ObjectFn data_801c5e74_slot04_0f[];
extern s32 data_801c5e7c_slot04_0f[];
extern s32 data_801c5e88_slot04_0f[];
extern ObjectFn data_801c5e94_slot04_0f[];
extern u8 data_801c605c_slot04_0f[];
extern ObjectFn data_801c6064_slot04_0f[];
extern ObjectFn data_801c6074_slot04_0f[];
extern ObjectFn data_801c6088_slot04_0f[];
extern ObjectFn data_801c6294_slot04_0f[];
extern ObjectRef ref_aux;

void call_handler_by_state_1_then_offset_slot04_0f(Object *obj);
void call_handler_by_state_1_then_offset_2_slot04_0f(Object *obj);
void copy_parent_position_with_offset_slot04_0f(Object *obj);
void set_sequence_by_sound_side_shifted_by_flag_slot04_0f(Object *obj, int a);
void call_handler_by_state_1_2_slot04_0f(Object *obj);
void call_state_2_handler_267c_unless_paused(Object *obj);
void call_state_2_handler_2684_unless_paused(Object *obj);
void call_state_2_handler_268c_unless_paused(Object *obj);
void call_handler_2694_by_state_2(Object *obj);
void reset_and_push_block_object_type_8(Slab172 *o);
void clear_or_set_flag_by_pos_x_limit(Object *o);

void update_moving_object_halving_velocity_on_flag_slot04_0f(Object *obj) {
    Config *config = game_state.config;
    u16 t;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        advance_sequence_checked(obj);
        t = obj->step_flags;
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            obj->vel_x >>= 1;
            obj->vel_y >>= 1;
        }
        if ((s16)obj->step_flags < 0) {
            obj->state_0 = 2;
            obj->state_1 = 0;
            obj->state_2 = 0;
            obj->state_3 = 0;
            call_handler_by_state_1_then_offset_slot04_0f(obj);
            return;
        }
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        set_state_0_if_x_outside_margin_narrow(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void call_state_2_handler_267c_wrapper_slot04_0f(Object *obj) {
    call_state_2_handler_267c_unless_paused(obj);
}

void call_state_2_handler_2684_wrapper_slot04_0f(Object *obj) {
    call_state_2_handler_2684_unless_paused(obj);
}

void call_handler_by_state_2_unless_paused_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        data_801c5e6c_slot04_0f[obj->state_2](obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void flip_facing_and_advance_state_2_slot04_0f(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    call_kind_9_handler_for_player(o);
    o->countdown = 2;
    advance_sequence_checked(o);
}

void count_down_then_reverse_velocity_x_slot04_0f(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->active_count = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->vel_y = 0;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
        set_sequence_by_sound_side(obj, obj->variant_index_2);
    }
    advance_sequence_checked(obj);
}

void call_handler_by_state_2_unless_paused_2_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        data_801c5e74_slot04_0f[obj->state_2](obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void flip_facing_and_advance_state_2_2_slot04_0f(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    call_kind_9_handler_for_player(o);
    o->countdown = 2;
    advance_sequence_checked(o);
}

void count_down_then_set_velocity_from_tables_slot04_0f(Object *obj) {
    int x;
    u8 i;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];

    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->active_count = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        i = obj->vel_table_index >> 1;
        x = data_801c5e7c_slot04_0f[i];
        obj->vel_y = data_801c5e88_slot04_0f[i];
        if (obj->vel_x >= 0) {
            x = -x;
        }
        obj->vel_x = x;
        set_sequence_by_sound_side(obj, obj->variant_index_2);
    } else {
        advance_sequence_checked(obj);
    }
}

void call_handler_by_state_1_then_offset_slot04_0f(Object *obj) {
    data_801c5e94_slot04_0f[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_1_clear_parent_link_set_sequence_slot04_0f(Object *obj) {
    obj->state_1++;
    obj->parent->linked_object_3 = 0;
    set_sequence_by_sound_side(obj, 0xb);
}

void set_state_0_to_3_on_sequence_end_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void release_object_and_unlink_from_parent_slot04_0f(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void call_handler_by_state_0_with_parent_slot04_0f(Object *obj) {
    ref_other.p = obj->parent;
    data_801c6064_slot04_0f[obj->state_0](obj);
}

void init_spawned_object_from_parent_slot04_0f(Object *obj) {
    obj->ot_depth = 0;
    obj->state_0++;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c605c_slot04_0f;
    obj->active_state = 0;
    copy_parent_position_with_offset_slot04_0f(obj);
    obj->variant_index_2 = 0xff;
    if (ref_other.p->side == 0) {
        ((Slot04bObj *)obj)->data_pointer = *(s32 *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->data_pointer = *(s32 *)0x1f800158;
    }
    set_sequence_by_sound_side_shifted_by_flag_slot04_0f(obj, obj->vel_table_index >> 1);
    call_handler_by_state_1_2_slot04_0f(obj);
}

void call_handler_by_state_1_2_slot04_0f(Object *obj) {
    data_801c6074_slot04_0f[obj->state_1](obj);
}

void update_attached_object_follow_parent_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        ref_other.p = obj->parent;
        if (*(u16 *)&ref_other.p->state_0 == 1 && ref_other.p->state_2 == 7) {
            advance_sequence_checked(obj);
            copy_parent_position_with_offset_slot04_0f(obj);
            if ((s16)obj->step_flags >= 0) {
                goto go;
            }
        }
        obj->state_0 = 2;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        call_handler_by_state_1_then_offset_2_slot04_0f(obj);
        return;
go:
        set_state_0_if_x_outside_margin_narrow(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void call_state_2_handler_267c_wrapper_2_slot04_0f(Object *obj) {
    call_state_2_handler_267c_unless_paused(obj);
}

void call_state_2_handler_2684_wrapper_2_slot04_0f(Object *obj) {
    call_state_2_handler_2684_unless_paused(obj);
}

void call_state_2_handler_268c_wrapper_slot04_0f(Object *obj) {
    call_state_2_handler_268c_unless_paused(obj);
}

void call_handler_2694_wrapper_slot04_0f(Object *obj) {
    call_handler_2694_by_state_2(obj);
}

void call_handler_by_state_1_then_offset_2_slot04_0f(Object *obj) {
    data_801c6088_slot04_0f[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_1_call_6_or_7_slot04_0f(Object *obj) {
    int n = 6;

    obj->state_1++;
    if (obj->table_index != 0) {
        n = 7;
    }
    set_sequence_by_sound_side_shifted_by_flag_slot04_0f(obj, n);
}

void set_state_0_to_3_on_sequence_end_2_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void unlink_from_parent_and_release_slot04_0f(Object *o) {
    ref_other.p = o->parent;
    ref_other.p->link_flags--;
    if (ref_other.p->link_flags == 0) {
        ref_other.p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void copy_parent_position_with_offset_slot04_0f(Object *obj) {
    int a;
    int b;

    ref_other.p = obj->parent;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&ref_other.p->pos_x_frac;
    a = -0x6c;
    b = -0x40;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&ref_other.p->pos_y_frac;
    if (obj->table_index != 0) {
        a = -0x4b;
        b = -0x80;
    }
    if (obj->facing_side != 0) {
        a = -a;
    }
    obj->pos_x = a + obj->pos_x;
    obj->pos_y = b + obj->pos_y;
}

void set_sequence_by_sound_side_shifted_by_flag_slot04_0f(Object *obj, int a) {
    if (obj->field_49 != 0) {
        a += 0x12;
    }
    set_sequence_by_sound_side(obj, a);
}

void call_handler_by_state_0_with_parent_2_slot04_0f(Object *obj) {
    ref_aux.p = obj->parent;
    data_801c6294_slot04_0f[obj->state_0](obj);
}
