/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ebc1c_slot06_0c[];
extern void (*data_801eaba4_slot06_0c[])(Object *, u16);
extern u16 data_801eabb8_slot06_0c[];
extern ObjectFn data_801eabc4_slot06_0c[];
extern u16 data_801eabd4_slot06_0c[];
extern u16 data_801eabd6_slot06_0c[];
extern u8 data_801eabdc_slot06_0c[];
extern ObjectFn data_801eabe0_slot06_0c[];

void start_sequence_14_if_player_state_2_is_7_slot06_0c(Object *obj);
void start_sequence_15_if_player_state_2_is_8_slot06_0c(Object *obj);
void set_position_from_variant_table_slot06_0c(Object *obj);
void set_sequence_by_variant_index_threshold_slot06_0c(Object *obj);

void func_801e9d74_slot06_0c(Object *obj, u16 unused) {
    int i;

    if (((s16)obj->step_flags & 0xff00) == 0) {
        advance_sequence_checked(obj);
        return;
    }
    if (obj->vel_x == 0x10) {
        obj->state_2 = 0;
        obj->state_1 = 0;
        advance_sequence_checked(obj);
        return;
    }
    if (obj->vel_x == 0x11) {
        obj->state_1 = 1;
        obj->state_2 = 2;
        i = 0xc;
    } else if (obj->vel_x == 0x12) {
        obj->state_2 = 2;
        obj->state_1 = 2;
        obj->facing_side = 1;
        i = 0xc;
    } else if (obj->vel_x == 0x13) {
        obj->state_1 = 1;
        obj->state_2 = 0;
        i = 0xb;
    } else {
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->facing_side = 1;
        i = 0xb;
    }
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
}

void call_handler_by_state_2_2_slot06_0c(Object *obj, int arg) {
    data_801eaba4_slot06_0c[obj->state_2](obj, arg);
}

void func_801e9e9c_slot06_0c(Object *obj, u16 arg) {
    int i;

    if (arg == 1) {
        start_sequence_14_if_player_state_2_is_7_slot06_0c(obj);
        start_sequence_15_if_player_state_2_is_8_slot06_0c(obj);
        advance_sequence_checked(obj);
        return;
    }
    i = 5;
    if (arg != 2) {
        obj->state_2 = 0;
        obj->state_1 = 0;
    } else {
        obj->vel_x = 0;
        obj->state_2++;
        i = 0xc;
    }
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
}

void set_state_2_sequence_by_vel_x_zero_slot06_0c(Object *obj, u16 unused) {
    int i = 0xb;
    int v;

    if (((s16)obj->step_flags & 0xff00) != 0) {
        v = 0;
        if (obj->vel_x == 0) {
            i = 0xd;
            v = 2;
        }
        obj->state_2 = v;
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
    } else {
        advance_sequence_checked(obj);
    }
}

void set_sequence_by_arg_1_or_2_else_reset_slot06_0c(Object *obj, u16 arg) {
    int i;

    if (arg == 2) {
        advance_sequence_checked(obj);
        return;
    }
    if (arg == 1) {
        obj->state_2 = 1;
        obj->vel_x = arg;
        i = 0xc;
    } else {
        obj->state_2 = 0;
        obj->state_1 = 0;
        i = 5;
    }
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
}

void start_sequence_from_arg_table_or_advance_slot06_0c(Object *obj, u16 arg) {
    if (((s16)obj->step_flags & 0xff00) != 0) {
        obj->state_2++;
        obj->vel_x = data_801eabb8_slot06_0c[arg];
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[data_801eabb8_slot06_0c[arg]]);
    } else {
        advance_sequence_checked(obj);
    }
}

void func_801ea0ac_slot06_0c(Object *obj, u16 unused) {
    int i;

    if (((s16)obj->step_flags & 0xff00) == 0) {
        advance_sequence_checked(obj);
        return;
    }
    if (obj->vel_x == 0x10) {
        obj->state_2 = 0;
        obj->state_1 = 0;
        advance_sequence_checked(obj);
        return;
    }
    if (obj->vel_x == 0x11) {
        obj->state_2 = 2;
        obj->state_1 = 2;
        i = 0xc;
    } else if (obj->vel_x == 0x12) {
        obj->state_1 = 1;
        obj->state_2 = 2;
        obj->facing_side = 0;
        i = 0xc;
    } else if (obj->vel_x == 0x13) {
        obj->state_1 = 2;
        obj->state_2 = 0;
        i = 0xb;
    } else {
        obj->state_1 = 1;
        obj->facing_side = 0;
        obj->state_2 = 0;
        i = 0xb;
    }
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[i]);
}

void start_sequence_14_if_player_state_2_is_7_slot06_0c(Object *obj) {
    if (player_left.state_2 == 7 || player_right.state_2 == 7) {
        obj->state_2 = 3;
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[14]);
    }
}

void start_sequence_15_if_player_state_2_is_8_slot06_0c(Object *obj) {
    if (player_left.state_2 == 8 || player_right.state_2 == 8) {
        obj->state_2 = 3;
        set_sequence_entry(obj, data_801ebc1c_slot06_0c[15]);
    }
}

int classify_object_x_against_players_slot06_0c(Object *obj) {
    int r;
    s16 rt;
    s16 l;
    u16 x;
    s16 lo;
    s16 hi;
    s16 d;

    rt = player_right.pos_x;
    l = player_left.pos_x;
    x = obj->pos_x;
    if (l <= rt) {
        lo = l;
        hi = rt;
    } else {
        lo = rt;
        hi = l;
    }
    d = hi - x;
    if (d >= 0) {
        d = lo - x;
        r = 0;
        if (d >= 0) {
            if (d < 0xc0) {
                r = 1;
            } else {
                r = 2;
            }
        }
    } else {
        if (d >= -0xbf) {
            r = 3;
        } else {
            r = 4;
        }
    }
    return r;
}

void release_object_record_2_slot06_0c(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_0c(Object *obj) {
    data_801eabc4_slot06_0c[obj->state_0](obj);
}

void init_variant_object_with_start_values_slot06_0c(Object *obj) {
    obj->state_0 = 1;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->vel_y = 0x1c0;
    obj->variant_index = 2;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    obj->vel_y -= 0x20;
    set_sequence_by_variant_index_threshold_slot06_0c(obj);
    set_position_from_variant_table_slot06_0c(obj);
}

void update_object_variant_by_scroll_slot06_0c(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->vel_y >= (s16)((Slot06Layer *)data_801aa5d4)->scroll_x) {
            if (obj->variant_index != 0) {
                obj->variant_index = 0;
                set_sequence_by_variant_index_threshold_slot06_0c(obj);
                set_position_from_variant_table_slot06_0c(obj);
            }
        } else if (obj->variant_index == 0) {
            obj->variant_index = 2;
            set_sequence_by_variant_index_threshold_slot06_0c(obj);
            set_position_from_variant_table_slot06_0c(obj);
        }
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void set_position_from_variant_table_slot06_0c(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    obj->pos_x = data_801eabd4_slot06_0c[obj->variant_index];
    obj->pos_y = 0x100 - data_801eabd6_slot06_0c[obj->variant_index];
}

void set_sequence_by_variant_index_threshold_slot06_0c(Object *obj) {
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[data_801eabdc_slot06_0c[obj->variant_index >= 2]]);
}

void release_object_record_3_slot06_0c(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_4_slot06_0c(Object *obj) {
    data_801eabe0_slot06_0c[obj->state_0](obj);
}

void init_object_vel_accel_from_position_word_slot06_0c(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->spawn_mode = 4;
    obj->vel_x = *(s32 *)&obj->pos_x_frac;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->state_0++;
    obj->accel_x = 0xf80000 - *(s32 *)&obj->pos_y_frac;
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[0]);
}

void set_object_position_from_layer_bounds_slot06_0c(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)data_801aa5d4;
    s32 a;

    a = *(s32 *)&l->bound_x_a - *(s32 *)&l->bound_x_c;
    a >>= 1;
    a += obj->vel_x;
    *(s32 *)&obj->pos_x_frac = a;
    a = *(s32 *)&l->bound_y_a - *(s32 *)&l->bound_y_c;
    a >>= 1;
    a += obj->accel_x;
    *(s32 *)&obj->pos_y_frac = 0x1000000 - a;
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_4_slot06_0c(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
