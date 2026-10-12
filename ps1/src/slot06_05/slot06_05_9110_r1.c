/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ec378_slot06_05[];
extern ObjectFn data_801ec388_slot06_05[];
extern ObjectFn data_801ec398_slot06_05[];
extern void (*data_801ec3a8_slot06_05[])(Object *obj, int arg);
extern SequenceStep *data_801ec344_slot06_05[];



Object *pop_object_from_second_pool(void);
int rand(void);
void set_sequence_entry(Object *object, SequenceStep *entry);
void func_8011f38c(Object *o);

void call_handler_by_state_0_slot06_05(Object *obj) {
    data_801ec378_slot06_05[obj->state_0](obj);
}

void init_object_oscillation_and_spawn_child_slot06_05(Object *obj) {
    Object *c;
    u32 x;
    obj->state_0 = obj->state_0 + 1;
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x18;
        c->table_index = 0;
        c->parent = obj;
        c->spawn_mode = 4;
        c->ot_depth = obj->ot_depth + 0xff;
        *(Object **)&obj->linked_object_2 = c;
    }
    x = *(u32 *)&obj->pos_x_frac;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->vel_x = 0x2000;
    obj->accel_x = -0x100;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->accel_y = 0;
    obj->spawn_mode = 4;
    obj->vel_y = x;
    set_sequence_entry(obj, data_801ec344_slot06_05[5]);
}

void update_object_oscillate_around_target_x_slot06_05(Object *obj) {
    int a;
    int b;
    int v;
    if ((game_state.field_65 | game_state.field_74) == 0) {
        *(u32 *)&obj->pos_x_frac += obj->vel_x;
        obj->vel_x += obj->accel_x;
        a = obj->vel_y;
        b = *(u32 *)&obj->pos_x_frac;
        if (a != b) {
            v = -(b < a);
            if (obj->accel_y != v) {
                obj->accel_y = v;
                if (v != 0) {
                    obj->vel_x = -0x2000;
                    obj->accel_x = 0x100;
                } else {
                    obj->vel_x = 0x2000;
                    obj->accel_x = -0x100;
                }
            }
        }
        if (*(s32 *)(data_801aa5d4 + 0x10) < 0x170) {
            return;
        }
        advance_sequence_checked(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_slot06_05(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot06_05(Object *obj) {
    data_801ec388_slot06_05[obj->state_0](obj);
}

void init_object_sequence_by_table_index_slot06_05(Object *obj) {
    int x = 0x800000;
    obj->field_0a = 1;
    obj->vel_x = 0x8000;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->accel_y = x;
    obj->state_0 = obj->state_0 + 1;
    obj->spawn_mode = 4;
    obj->vel_y = *(u32 *)&obj->pos_x_frac - x;
    set_sequence_entry(obj, data_801ec344_slot06_05[obj->table_index]);
}

void update_object_x_from_masked_accumulator_slot06_05(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        obj->vel_y += obj->vel_x;
        *(u32 *)&obj->pos_x_frac = (obj->vel_y & 0x3ff0000) + obj->accel_y;
    }
    func_8011ffdc(obj);
}

void release_object_record_2_slot06_05(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_05(Object *obj) {
    data_801ec398_slot06_05[obj->state_0](obj);
}

void init_child_from_parent_position_slot06_05(Object *obj) {
    Object *p = obj->parent;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->field_01 = 1;
    obj->state_0 = 1;
    obj->field_0e = p->field_0e;
    obj->field_0f = p->field_0f;
    obj->field_1c = p->field_1c;
    obj->scroll_factor = p->scroll_factor;
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
    set_sequence_entry(obj, data_801ec344_slot06_05[6]);
}

void call_state_handler_then_follow_parent_slot06_05(Object *obj) {
    Object *p = obj->parent;
    s16 a;
    s16 b;
    s16 lo;
    s16 hi;
    int x;
    int r;
    if (game_state.field_65 == 0) {
        b = player_left.pos_x;
        a = player_right.pos_x;
        if (b <= a) { lo = b; hi = a; } else { lo = a; hi = b; }
        x = *(u16 *)&obj->pos_x;
        r = 0;
        if ((s16)(x - lo + 0x50) >= 0xa1 && (s16)(x - hi + 0x50) >= 0xa1) {
            x = (s16)x;
            r = 2;
            if ((s16)lo < x) {
                r = !(x < (s16)hi);
            }
        }
        data_801ec3a8_slot06_05[obj->state_1](obj, r);
    }
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
}

void select_sequence_by_arg_or_random_slot06_05(Object *obj, int arg) {
    int i;
    if ((s16)obj->step_flags & 0x8000) {
        if ((u8)arg != obj->state_1) {
            i = (u8)(arg + 6) << 2;
            obj->state_1 = arg;
        } else {
            if ((rand() & 0x7f) != 0) {
                goto skip;
            }
            i = ((rand() & 1) + 0xb) << 2;
        }
        set_sequence_entry(obj, *(SequenceStep **)((u8 *)data_801ec344_slot06_05 + i));
    } else {
skip:
        advance_sequence_checked(obj);
    }
}

void reset_states_on_arg_mismatch_sequence_8_slot06_05(Object *obj, int arg) {
    if (((s16)obj->step_flags & 0x8000) && (u8)arg != obj->state_1) {
        obj->state_0 = 1;
        obj->state_3 = 0;
        obj->state_2 = 0;
        obj->state_1 = 0;
        set_sequence_entry(obj, data_801ec344_slot06_05[8]);
    } else {
        advance_sequence_checked(obj);
    }
}

void restart_sequence_10_if_arg_changed_slot06_05(Object *obj, int arg) {
    if (((s16)obj->step_flags & 0x8000) && (u8)arg != obj->state_1) {
        obj->state_0 = 1;
        obj->state_3 = 0;
        obj->state_2 = 0;
        obj->state_1 = 0;
        set_sequence_entry(obj, data_801ec344_slot06_05[10]);
    } else {
        advance_sequence_checked(obj);
    }
}

void unlink_from_parent_and_release_object_slot06_05(Object *obj) {
    Object *p = obj->parent;
    if (p->linked_object == (u32)obj) {
        p->linked_object = 0;
    } else if (*(Object **)&p->linked_object_2 == obj) {
        *(u32 *)&p->linked_object_2 = 0;
    } else if (((Slot06Obj *)p)->field_30 == (s32)obj) {
        ((Slot06Obj *)p)->field_30 = 0;
    } else if (p->linked_object_b == (u32)obj) {
        p->linked_object_b = 0;
    }
    func_8011f38c(obj);
}
