/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void *data_801c7b58_slot04_11[];
extern void *data_801c7bd0_slot04_11[];
extern ObjectFn data_801c7bf0_slot04_11[];
extern Slot04_11Rec7bfc data_801c7bfc_slot04_11[];
extern u8 data_801c7c5c_slot04_11[];
extern Object *data_801c7cc0_slot04_11[];
extern ObjectFn data_801c7cd0_slot04_11[];
extern Slot04_11Rec7cf0 data_801c7cf0_slot04_11[];
extern u16 data_801c7d10_slot04_11[];
extern u16 data_801a2944[];
extern u16 data_801a29e4[];

Pooled *alloc_pooled_b(void);
Object *pop_object_from_second_pool(void);
void reset_and_push_object_to_pool_table(Slab172 *s);

void queue_side_record_at_offset_position_slot04_11(Object *obj);
void noop_slot00_slot04_11(Object *obj);
void set_position_above_other_by_kind_slot04_11(Object *obj);
void reset_countdown_and_set_indicator_boxes_slot04_11(Object *obj);
void init_side_record_and_box_tables_slot04_11(Object *obj);
void queue_palette_upload_and_copy_for_side_slot04_11(Object *obj);
void call_side_routine_and_set_side_flag_slot04_11(Object *obj);
Object *get_table_object_by_field_2_slot04_11(Object *obj);
void follow_other_by_frame_param_slot04_11(Object *obj);
void increment_state_0_slot04_11(Object *obj);
void clear_variant_index_and_first_byte_slot04_11(Object *obj);
void set_variant_sequence_from_side_table_slot04_11(Object *obj, int idx);
void clear_object_and_push_on_free_stack(Object *o);

void call_state_1_handler_or_advance_state_0_slot04_11(Object *obj) {
    Object *s = obj->other;

    set_position_above_other_by_kind_slot04_11(obj);
    if ((*(u32 *)&s->field_28c & 0x80ff) != 0x8000 && (game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) == 0) {
        data_801c7bf0_slot04_11[obj->state_1](obj);
        queue_side_record_at_offset_position_slot04_11(obj);
    } else {
        obj->state_0++;
        call_side_routine_and_set_side_flag_slot04_11(obj);
    }
}

void queue_side_record_at_offset_position_slot04_11(Object *obj) {
    Slot04_11Rec7cf0 *r = &data_801c7cf0_slot04_11[obj->other->side];
    Object *t = get_table_object_by_field_2_slot04_11(ref_other.p);

    r->c = obj->box_tables;
    r->x = obj->pos_x - t->pos_x - 8;
    r->y = obj->pos_y + t->pos_y;
    add_item_to_table((Object *)r);
}

void update_track_other_value_spawn_child_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *n;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        obj->field_47 = k;
        queue_palette_upload_and_copy_for_side_slot04_11(o);
        play_side_sound_at(ref_other.p->other, ref_other.p->other->side, 0xf);
        reset_countdown_and_set_indicator_boxes_slot04_11(o);
    } else {
        obj->countdown--;
        if (obj->countdown == 0 && data_801a4fec != 0) {
            obj->countdown = 0xc;
            o->state_1++;
            n = pop_object_from_second_pool();
            if (n != 0) {
                n->active_count = 1;
                n->field_02 = 0x1e;
                n->field_0e = o->field_0e;
                n->variant_index = 0;
                n->parent = o;
                n->table_index = 0;
                n->facing_side = o->facing_side;
                n->other = o->other;
                o->linked_object_b = (u32)n;
                n->base_x = 0x240;
                n->spawn_x = 0x60;
                n->spawn_y = 0x1e2;
                n->data_ptr_a = data_80172a48;
                n->base_y = 0;
                n->field_90 = (void *)0x800fb100;
                n->data_ptr_b = data_80173c9c;
                n->sound_side = o->sound_side;
            }
        }
    }
}

void update_selection_change_countdown_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        o->state_1 = 0;
        obj->field_47 = k;
        queue_palette_upload_and_copy_for_side_slot04_11(o);
        play_side_sound_at(ref_other.p->other, ref_other.p->other->side, 0xf);
        reset_countdown_and_set_indicator_boxes_slot04_11(o);
    } else {
        obj->countdown--;
        if (obj->countdown == 0) {
            o->state_1++;
            init_side_record_and_box_tables_slot04_11(o);
        } else {
            noop_slot00_slot04_11(o);
        }
    }
}

void noop_slot00_slot04_11(Object *obj) {
}

void update_reset_on_other_value_change_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 == k) {
        noop_slot00_slot04_11(o);
    } else {
        o->state_1 = 0;
        obj->field_47 = k;
        queue_palette_upload_and_copy_for_side_slot04_11(o);
        play_side_sound_at(ref_other.p->other, ref_other.p->other->side, 0xf);
        reset_countdown_and_set_indicator_boxes_slot04_11(o);
    }
}

void unlink_parent_and_advance_linked_state_0_slot04_11(Object *obj) {
    obj->state_0++;
    ref_other.p = obj->parent;
    if (ref_other.p->parent == obj) {
        ref_other.p->parent = 0;
    }
    if (obj->linked_object_b != 0) {
        ref_other.p = (Object *)obj->linked_object_b;
        if (obj == ref_other.p->parent && ref_other.p->active_count != 0) {
            ref_other.p->state_0++;
            obj->linked_object_b = 0;
        }
    }
}

void release_object_record_slot04_11(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_position_above_other_by_kind_slot04_11(Object *obj) {
    int d = data_801c7bfc_slot04_11[ref_other.p->kind].b;

    obj->pos_x = ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y - d - 0x18;
}

void reset_countdown_and_set_indicator_boxes_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_11Rec7cf0 *r;

    obj->countdown = 5;
    r = &data_801c7cf0_slot04_11[o->other->side];
    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    o->box_tables = data_801c7b58_slot04_11[obj->field_47];
}

void copy_zero_terminated_bytes_slot04_11(u8 *src, u8 *dst) {
    if (*src != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
        } while (*src != 0);
        *dst = 0;
    }
}

void init_side_record_and_box_tables_slot04_11(Object *obj) {
    Slot04_11Rec7cf0 *r = &data_801c7cf0_slot04_11[ref_other.p->side];

    r->a = 0;
    r->b8 = 0x10;
    r->b9 = 0x10;
    r->ba = 2;
    r->bb = 0x10;
    obj->box_tables = data_801c7bd0_slot04_11[0];
    call_side_routine_and_set_side_flag_slot04_11(obj);
}

int get_table_entry_by_index_slot04_11(u16 index) {
    return data_801c7c5c_slot04_11[index];
}

void queue_palette_upload_and_copy_for_side_slot04_11(Object *obj) {
    u16 *s = data_801c7d10_slot04_11 + ref_other.p->side * 16;
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
        job->src = (u8 *)(data_801c7d10_slot04_11 + ref_other.p->side * 16);
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

void store_side_vram_row_to_buffer_slot04_11(Object *obj) {
    Object *p = ref_other.p;
    Rect rect;

    rect.x = p->side * 32 + 0x110;
    rect.y = 0x1e0;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, (u8 *)(data_801c7d10_slot04_11 + p->side * 16));
}

void call_side_routine_and_set_side_flag_slot04_11(Object *obj) {
    if (game_state.field_65 == 0) {
        if (ref_other.p->side == 0) {
            store_player_left_image_rows_from_field();
        } else {
            store_player_right_image_rows_from_field();
        }
        set_side_flag(ref_other.p);
    }
}

void noop_slot00_2_slot04_11(Object *obj) {
}

Object *get_table_object_by_field_2_slot04_11(Object *obj) {
    return data_801c7cc0_slot04_11[obj->field_0e];
}

void call_handler_by_state_0_with_parent_slot04_11(Object *obj) {
    ref_other.p = obj->parent;
    data_801c7cd0_slot04_11[obj->state_0](obj);
}

void advance_state_0_copy_other_fields_slot04_11(Object *obj) {
    obj->state_0++;
    obj->field_1c = ref_other.p->field_1c;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->variant_index = 0;
    obj->field_01 = 0;
    follow_other_by_frame_param_slot04_11(obj);
}

void follow_other_by_frame_param_slot04_11(Object *obj) {
    Object *p;
    u8 f;

    obj->field_01 = 0;
    if (ref_other.p->kind != 0x11 && ref_other.p->kind != 0x13) {
        increment_state_0_slot04_11(obj);
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
            set_variant_sequence_from_side_table_slot04_11(obj, f);
        }
    } else {
        clear_variant_index_and_first_byte_slot04_11(obj);
    }
}

void increment_state_0_slot04_11(Object *obj) {
    obj->state_0++;
}

void clear_other_link_and_release_object_slot04_11(Object *obj) {
    ref_other.p->linked_object = 0;
    clear_object_and_push_on_free_stack(obj);
}

void clear_variant_index_and_first_byte_slot04_11(Object *obj) {
    obj->variant_index = 0;
    obj->field_01 = 0;
}
