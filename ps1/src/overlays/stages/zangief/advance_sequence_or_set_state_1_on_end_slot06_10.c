/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ea998_slot06_10[];
extern void (*data_801ea9a8_slot06_10[])(Object *, Object *);
extern ObjectFn data_801ea9b4_slot06_10[];
extern void (*data_801ea9c4_slot06_10[])(Object *, Object *);
extern SequenceStep *data_801ebbc4_slot06_10[];
extern s16 data_801f67f0_slot06_10;
extern s16 data_801f67f4_slot06_10;

void store_ordered_pos_x_by_facing_side_slot06_10(Object *obj, Object *other);

void advance_sequence_or_set_state_1_on_end_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 1;
    } else {
        advance_sequence_checked(obj);
    }
}

void advance_sequence_checked_wrapper_slot06_10(Object *obj, Object *other) {
    advance_sequence_checked(obj);
}

void set_state_1_3_if_pause_flag_2_slot06_10(Object *obj, int unused, u8 index) {
    if (game_state.pause_flag_2 != 0) {
        obj->state_1 = 3;
        set_sequence_entry(obj, data_801ebbc4_slot06_10[index]);
    }
}

void start_sequence_1_when_other_crosses_x_slot06_10(Object *obj, Object *p) {
    u16 x = p->pos_x;

    if (obj->table_index != 0) {
        if ((s16)obj->pos_x < (s16)x) {
            return;
        }
    } else {
        if ((s16)x < (s16)obj->pos_x) {
            return;
        }
    }
    obj->state_1 = 1;
    set_sequence_entry(obj, data_801ebbc4_slot06_10[1]);
}

void start_sequence_2_when_other_crosses_back_slot06_10(Object *obj, Object *p) {
    u16 x = p->pos_x;

    if (obj->table_index != 0) {
        if ((s16)x < (s16)obj->pos_x) {
            return;
        }
    } else {
        if ((s16)obj->pos_x < (s16)x) {
            return;
        }
    }
    obj->state_1 = 0;
    obj->countdown = 0;
    set_sequence_entry(obj, data_801ebbc4_slot06_10[2]);
}

void count_up_then_start_sequence_3_slot06_10(Object *obj, Object *other) {
    obj->countdown = (s16)obj->countdown + 1;
    if ((s16)obj->countdown >= 0x258) {
        obj->countdown = 0;
        obj->state_1 = 2;
        set_sequence_entry(obj, data_801ebbc4_slot06_10[3]);
    }
}

void release_object_record_3_slot06_10(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_4_slot06_10(Object *obj) {
    data_801ea998_slot06_10[obj->state_0](obj);
}

void init_object_with_sequence_table_entry_5_slot06_10(Object *obj) {
    u16 y;

    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->base_x = 0x340;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    y = obj->pos_y;
    obj->state_1 = 0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->pos_y = y + 0x70;
    set_sequence_entry(obj, data_801ebbc4_slot06_10[5]);
}

void call_state_1_handler_with_player_b_slot06_10(Object *obj) {
    Object *p;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        p = &player_left;
        if (obj->table_index != 0) {
            p = p + 1;
        }
        data_801ea9a8_slot06_10[obj->state_1](obj, p);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void wait_for_other_x_then_start_sequence_6_slot06_10(Object *obj, Object *p) {
    u16 x;

    if ((s16)obj->step_flags & 0x8000) {
        x = p->pos_x;
        if (obj->table_index != 0) {
            if ((s16)x < 0x300) {
                return;
            }
        } else {
            if ((s16)x >= 0x201) {
                return;
            }
        }
        obj->state_1 = 1;
        set_sequence_entry(obj, data_801ebbc4_slot06_10[6]);
    } else {
        advance_sequence_checked(obj);
    }
}

void choose_sequence_by_other_x_on_flag_slot06_10(Object *obj, Object *p) {
    u16 x;

    if (obj->step_flags & 0x800) {
        x = p->pos_x;
        if (obj->table_index != 0) {
            if (obj->pos_x <= (s16)x) {
                obj->state_1 = 2;
                set_sequence_entry(obj, data_801ebbc4_slot06_10[7]);
            } else if ((s16)x < 0x301) {
                obj->state_1 = 0;
                set_sequence_entry(obj, data_801ebbc4_slot06_10[5]);
            }
        } else {
            if (obj->pos_x >= (s16)x) {
                obj->state_1 = 2;
                set_sequence_entry(obj, data_801ebbc4_slot06_10[7]);
            } else if ((s16)x >= 0x200) {
                obj->state_1 = 0;
                set_sequence_entry(obj, data_801ebbc4_slot06_10[5]);
            }
        }
    } else {
        advance_sequence_checked(obj);
    }
}

void start_sequence_8_when_other_crosses_x_slot06_10(Object *obj, Object *p) {
    u16 x;

    if ((s16)obj->step_flags & 0x8000) {
        x = p->pos_x;
        if (obj->table_index != 0) {
            if ((s16)obj->pos_x < (s16)x) {
                return;
            }
        } else {
            if ((s16)x < (s16)obj->pos_x) {
                return;
            }
        }
        obj->state_1 = 1;
        set_sequence_entry(obj, data_801ebbc4_slot06_10[8]);
    } else {
        advance_sequence_checked(obj);
    }
}

void release_object_record_4_slot06_10(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_5_slot06_10(Object *obj) {
    data_801ea9b4_slot06_10[obj->state_0](obj);
}

void init_object_with_sequence_table_entry_9_slot06_10(Object *obj) {
    u16 y;

    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->base_x = 0x340;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    y = obj->pos_y;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->pos_y = y + 0x70;
    set_sequence_entry(obj, data_801ebbc4_slot06_10[9]);
}

void call_state_1_handler_by_facing_side_slot06_10(Object *obj) {
    Object *p;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        p = &player_left;
        if (obj->facing_side != 0) {
            p = p + 1;
        }
        data_801ea9c4_slot06_10[obj->state_1](obj, p);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_1_if_x_order_ascending_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->step_flags & 0x8000) {
        store_ordered_pos_x_by_facing_side_slot06_10(obj, other);
        if (data_801f67f0_slot06_10 < data_801f67f4_slot06_10) {
            obj->state_1 = obj->state_1 + 1;
            set_sequence_entry(obj, data_801ebbc4_slot06_10[10]);
            return;
        }
    }
    advance_sequence_checked(obj);
}
