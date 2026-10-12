/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void *data_801ce868_slot04_17[];
extern void *data_801ce8e0_slot04_17[];
extern Slot04_17Rece90c data_801ce90c_slot04_17[];
extern u8 data_801ce96c_slot04_17[];
extern Object *data_801ce9d0_slot04_17[];
extern ObjectFn data_801ce9e0_slot04_17[];
extern Slot04_17Recec3c data_801cec3c_slot04_17[];
extern u16 data_801cec5c_slot04_17[];
extern u16 data_801a2944[];
extern u16 data_801a29e4[];

Pooled *alloc_pooled_b(void);
void reset_and_push_object_to_pool_table(Slab172 *s);
void func_8011f38c(Object *o);

void noop_slot00_slot04_17(Object *obj);
void release_object_record_slot04_17(Object *obj);
void reset_countdown_and_set_indicator_boxes_slot04_17(Object *obj);
void init_side_record_and_box_tables_slot04_17(Object *obj);
void queue_palette_upload_and_copy_for_side_slot04_17(Object *obj);
void call_side_routine_and_set_side_flag_slot04_17(Object *obj);
void follow_other_by_frame_param_slot04_17(Object *obj);
void increment_state_0_slot04_17(Object *obj);
void func_801b6ad0_slot04_17(Object *obj);
void set_variant_sequence_from_side_table_slot04_17(Object *obj, int index);

void noop_slot00_slot04_17(Object *obj) {
}

void update_reset_on_other_value_change_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 == k) {
        noop_slot00_slot04_17(o);
    } else {
        o->state_1 = 0;
        obj->field_47 = k;
        queue_palette_upload_and_copy_for_side_slot04_17(o);
        play_side_sound_at(ref_other.p->other, ref_other.p->other->side, 0xf);
        reset_countdown_and_set_indicator_boxes_slot04_17(o);
    }
}

void end_and_unlink_parent_and_linked_object_slot04_17(Object *obj) {
    obj->state_0++;
    ref_other.p = obj->parent;
    if (ref_other.p->parent == obj) {
        ref_other.p->parent = 0;
    }
    if (obj->linked_object_b != 0) {
        ref_other.p = (Object *)obj->linked_object_b;
        if (obj == ref_other.p->parent) {
            ref_other.p->state_0++;
            obj->linked_object_b = 0;
        }
    }
}

void release_object_record_slot04_17(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_position_above_other_by_kind_slot04_17(Object *obj) {
    int d = data_801ce90c_slot04_17[ref_other.p->kind].b;

    obj->pos_x = ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y - d - 0x18;
}

void reset_countdown_and_set_indicator_boxes_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_17Recec3c *r;

    obj->countdown = 5;
    r = &data_801cec3c_slot04_17[o->other->side];
    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    o->box_tables = data_801ce868_slot04_17[obj->field_47];
}

void copy_zero_terminated_bytes_slot04_17(u8 *src, u8 *dst) {
    if (*src != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
        } while (*src != 0);
        *dst = 0;
    }
}

void init_side_record_and_box_tables_slot04_17(Object *obj) {
    Slot04_17Recec3c *r = &data_801cec3c_slot04_17[ref_other.p->side];

    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    obj->box_tables = data_801ce8e0_slot04_17[0];
    call_side_routine_and_set_side_flag_slot04_17(obj);
}

int get_table_entry_by_index_slot04_17(u16 index) {
    return data_801ce96c_slot04_17[index];
}

void queue_palette_upload_and_copy_for_side_slot04_17(Object *obj) {
    u16 *s = data_801cec5c_slot04_17 + ref_other.p->side * 16;
    Job *job;
    Rect rect;
    u16 *p;
    u16 *q;
    int i;

    job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = ref_other.p->image_row + 0x1e0;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = (u8 *)(data_801cec5c_slot04_17 + ref_other.p->side * 16);
        i = 0;
        p = data_801a2944;
        if (ref_other.p->side == 0) {
            p = data_801a29e4;
        }
        q = p + 0xa00;
        do {
            *p = *s;
            p++;
            *q = *s;
            s++;
            q++;
            i++;
        } while (i < 0x10);
    }
}

void func_801b67f0_slot04_17(Object *obj) {
    Object *p = ref_other.p;
    Rect rect;

    rect.x = p->side * 32 + 0x110;
    rect.y = 0x1e0;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, (u8 *)(data_801cec5c_slot04_17 + p->side * 16));
}

void call_side_routine_and_set_side_flag_slot04_17(Object *obj) {
    if (game_state.field_65 == 0) {
        if (ref_other.p->side == 0) {
            func_80136e90();
        } else {
            func_80136ed0();
        }
        set_side_flag(ref_other.p);
    }
}

void noop_slot00_2_slot04_17(Object *obj) {
}

Object *func_801b68c4_slot04_17(Object *obj) {
    return data_801ce9d0_slot04_17[obj->field_0e];
}

void call_handler_by_state_0_with_parent_slot04_17(Object *obj) {
    ref_other.p = obj->parent;
    data_801ce9e0_slot04_17[obj->state_0](obj);
}

void advance_state_0_copy_other_fields_slot04_17(Object *obj) {
    obj->state_0++;
    obj->field_1c = ref_other.p->field_1c;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->variant_index = 0;
    obj->field_01 = 0;
    follow_other_by_frame_param_slot04_17(obj);
}

void follow_other_by_frame_param_slot04_17(Object *obj) {
    Object *p;
    u8 f;

    obj->field_01 = 0;
    if (ref_other.p->kind != 0x11 && ref_other.p->kind != 0x13) {
        increment_state_0_slot04_17(obj);
    }
    p = ref_other.p;
    f = p->frame->frame_param;
    if (f != 0) {
        obj->pos_x = p->pos_x;
        obj->pos_y = ref_other.p->pos_y;
        obj->facing_side = ref_other.p->facing_side;
        obj->field_01 = 1;
        if (obj->variant_index == f) {
            advance_sequence_checked(obj);
        } else {
            set_variant_sequence_from_side_table_slot04_17(obj, f);
        }
    } else {
        func_801b6ad0_slot04_17(obj);
    }
}

void increment_state_0_slot04_17(Object *obj) {
    obj->state_0++;
}

void clear_other_link_and_release_object_slot04_17(Object *obj) {
    ref_other.p->linked_object = 0;
    func_8011f38c(obj);
}

void func_801b6ad0_slot04_17(Object *obj) {
    obj->variant_index = 0;
    obj->field_01 = 0;
}
