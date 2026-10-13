/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern Object *data_801dd754_slot05_06;
void set_variant_sequence_by_sound_side_slot05_06(Object *obj, int a);
void increment_state_0_slot05_06(Object *obj);
extern void (*data_801dd5c0_slot05_06[])(Object *);

void follow_parent_position_and_sequence_slot05_06(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801dd754_slot05_06;
    if (obj->table_index == p->kind) {
        a = p->frame->frame_param;
        if (a == 0) {
            obj->variant_index = 0;
        } else {
            obj->pos_x = p->pos_x;
            obj->pos_y = p->pos_y;
            obj->facing_side = p->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != a) {
                set_variant_sequence_by_sound_side_slot05_06(obj, a);
            } else {
                advance_sequence_checked(obj);
            }
        }
    } else {
        increment_state_0_slot05_06(obj);
    }
}

void increment_state_0_slot05_06(Object *o) {
    o->state_0++;
}

void release_object_unlink_global_link_slot05_06(Object *o) {
    Object *p = data_801dd754_slot05_06;

    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}

void clear_variant_index_slot05_06(Object *o) {
    o->variant_index = 0;
}

void set_variant_sequence_by_sound_side_slot05_06(Object *obj, int idx) {
    SequenceStep **table;

    obj->variant_index = idx;
    if (obj->sound_side == 0) {
        table = seqs_b4_left;
    } else {
        table = seqs_164_right;
    }
    set_sequence_from_table_5(obj, (u8)idx, table);
}

void call_handler_by_state_0_slot05_06(Object *obj) {
    data_801dd5c0_slot05_06[obj->state_0](obj);
}
