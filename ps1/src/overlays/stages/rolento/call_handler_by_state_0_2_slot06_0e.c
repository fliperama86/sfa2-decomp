/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801ef40c_slot06_0e[];
extern SequenceStep *data_801ef410_slot06_0e[];
extern SequenceStep *data_801ef414_slot06_0e[];
extern SequenceStep *data_801ef424_slot06_0e;
extern SequenceStep *data_801ef42c_slot06_0e;
extern SequenceStep *data_801ef430_slot06_0e[];
extern ObjectFn data_801ef434_slot06_0e[];
extern ObjectFn data_801ef444_slot06_0e[];
extern ObjectFn data_801ef44c_slot06_0e[];
extern ObjectFn data_801ef45c_slot06_0e[];
extern ObjectFn data_801ef464_slot06_0e[];
extern ObjectFn data_801ef474_slot06_0e[];
extern ObjectFn data_801ef484_slot06_0e[];

Object *pop_object_from_second_pool(void);
void clear_object_and_push_on_free_stack(Object *o);

void call_handler_by_state_0_2_slot06_0e(Object *obj) {
    data_801ef434_slot06_0e[obj->state_0](obj);
}

void init_object_state_and_first_sequence_a_slot06_0e(Object *obj) {
    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->pos_y_frac = 0;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    obj->variant_index_2 = 0;
    set_sequence_entry(obj, data_801ef414_slot06_0e[0]);
}

void update_object_side_from_layer_scroll_slot06_0e(Object *obj) {
    Slot06Layer *layer = (Slot06Layer *)data_801aa5d4;
    u8 side;
    s16 x;
    if (layer->scroll_x > layer->ref_x) {
        side = 1;
        x = 0x381;
        if (obj->variant_index_2 != 1) {
            set_sequence_entry(obj, data_801ef42c_slot06_0e);
        }
    } else {
        side = 0;
        x = 0x17f;
        if (obj->variant_index_2 != 0) {
            set_sequence_entry(obj, data_801ef414_slot06_0e[0]);
        }
    }
    obj->pos_x = x;
    obj->facing_side = side;
    data_801ef444_slot06_0e[obj->state_1](obj);
    set_flag_and_push_if_mode_8(obj);
    obj->variant_index_2 = obj->facing_side;
}

void update_object_pos_y_reflect_and_advance_slot06_0e(Object *obj) {
    int t;
    s16 d;
    s16 u;
    t = game_state.field_65 | game_state.field_74;
    if ((t | ((Slot06Layer *)cam_obj)->field_8b) == 0) {
        if (data_801aa548[1] == 2) {
            obj->state_1++;
        }
        d = 0xf8 - (u16)obj->pos_y;
        u = d - 1;
        obj->pos_y = 0xf8 - u;
        if ((u16)u < 0x79) {
            u = d + 0xf;
            obj->pos_y = 0xf8 - u;
            advance_sequence_checked(obj);
        }
    }
}

void noop_slot00_2_slot06_0e(Object *obj) {
}

void release_object_record_2_slot06_0e(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_0e(Object *obj) {
    data_801ef44c_slot06_0e[obj->state_0](obj);
}

void spawn_child_and_advance_state_0_slot06_0e(Object *obj) {
    Object *c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x15;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        c->scroll_factor = obj->scroll_factor;
        c->field_1c = obj->field_1c;
        c->parent = obj;
        c->spawn_mode = 4;
        c->ot_depth = obj->ot_depth;
        *(Object **)&obj->linked_object_2 = c;
    }
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->state_0 = obj->state_0 + 1;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    set_sequence_entry(obj, data_801ef40c_slot06_0e[0]);
}

void call_handler_by_state_1_then_offset_slot06_0e(Object *obj) {
    data_801ef45c_slot06_0e[obj->state_1](obj);
    set_flag_and_push_if_mode_8(obj);
}

void scroll_object_pos_y_down_with_wrap_slot06_0e(Object *obj) {
    s16 y;
    int t;
    t = game_state.field_65 | game_state.field_74;
    if ((t | ((Slot06Layer *)cam_obj)->field_8b) == 0) {
        if (data_801aa548[1] == 2) {
            obj->state_1++;
        }
        y = (u16)obj->pos_y;
        y += 1;
        obj->pos_y = y;
        if (y >= 0x130) {
            obj->pos_y = -0xd7;
        }
    }
}

void noop_slot00_3_slot06_0e(Object *obj) {
}

void release_object_record_3_slot06_0e(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_4_slot06_0e(Object *obj) {
    data_801ef464_slot06_0e[obj->state_0](obj);
}

void init_object_state_and_first_sequence_b_slot06_0e(Object *obj) {
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->state_0 = 1;
    obj->field_01 = 1;
    obj->spawn_mode = 4;
    obj->field_0f = 1;
    set_sequence_entry(obj, data_801ef410_slot06_0e[0]);
}

void place_object_side_by_bounds_at_parent_y_slot06_0e(Object *obj) {
    Slot06Layer *layer = (Slot06Layer *)data_801aa5d4;
    Object *p = obj->parent;
    if (*(u32 *)&layer->bound_x_b > *(u32 *)&layer->bound_x_a) {
        obj->facing_side = 1;
        obj->pos_x = 0x3b0;
        set_sequence_entry(obj, data_801ef430_slot06_0e[0]);
    } else {
        obj->facing_side = 0;
        obj->pos_x = 0x150;
        set_sequence_entry(obj, data_801ef410_slot06_0e[0]);
    }
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y - 8;
}

void load_velocities_from_table_slot06_0e(Object *obj) {
    clear_object_and_push_on_free_stack(obj);
}

void call_handler_by_state_0_5_slot06_0e(Object *obj) {
    data_801ef474_slot06_0e[obj->state_0](obj);
}

void init_object_from_parent_position_slot06_0e(Object *obj) {
    Object *p = obj->parent;
    obj->state_0 = obj->state_0 + 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->field_0e = p->field_0e;
    obj->field_0f = p->field_0f;
    obj->field_1c = p->field_1c;
    obj->scroll_factor = p->scroll_factor;
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
    obj->spawn_mode = 4;
    set_sequence_entry(obj, data_801ef424_slot06_0e);
}

void copy_parent_position_and_advance_sequence_slot06_0e(Object *obj) {
    Object *p = obj->parent;
    if (obj->field_01 != 0) {
        *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
        *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
        advance_sequence_checked(obj);
    }
}

void unlink_from_parent_and_release_object_slot06_0e(Object *obj) {
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
    clear_object_and_push_on_free_stack(obj);
}

void call_handler_by_state_0_6_slot06_0e(Object *obj) {
    data_801ef484_slot06_0e[obj->state_0](obj);
}
