/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
void clear_object_fields_slot27(Object *obj);
void clear_side_record_counters_slot27(void);
void set_sequence_and_position_by_other_kind_slot27(Object *obj);
void load_palette_anim_from_table_index_slot27(Object *obj);

void init_selection_object_for_active_other_side_slot27(Object *obj) {
    int *state = (int *)scratch_word_a;
    int *mask = scratch_word_b;

    int t = game_state.mode;
    int u;
    int side;

    *state = t;
    u = game_state.side_mask | t;
    *state = u;
    side = ref_other.p->side;
    *mask = side;
    if (((u >> side) & 1) != 0) {
        *mask = 1 << ref_other.p->side;
        clear_object_fields_slot27(obj);
        clear_side_record_counters_slot27();
        obj->ot_depth = 4;
        obj->field_90 = (void *)0x80038000;
        obj->data_ptr_a = data_80017c28_slot27;
        obj->data_ptr_b = data_8001aa14_slot27;
        obj->spawn_x = 0;
        obj->spawn_y = 0x1e0;
        obj->image_row = 0;
        obj->field_01 = 0;
        obj->facing_side = 0;
        obj->ground_y = 0;
        obj->status_flag = 0;
        obj->state_0++;
        if ((game_state.side_mask & *mask) != 0) {
            *state = ref_other.p->kind;
            if (*state == 0x12) {
                *state = 4;
                ref_other.p->kind = *state;
            }
            if (*state == 0x13) {
                *state = 0x11;
                ref_other.p->kind = *state;
            }
            if (*state == 0x14) {
                *state = 2;
                ref_other.p->kind = *state;
            }
            obj->table_index = *state;
            obj->countdown = 0x4b0;
            set_sequence_and_position_by_other_kind_slot27(obj);
            ref_first.p = ref_other.p;
            ref_other.p = pop_object_from_second_pool();
            if (ref_other.p != 0) {
                ref_other.p->active_count = 1;
                ref_other.p->field_02 = 0x24;
                ref_other.p->other = ref_first.p;
                ref_other.p->parent = obj;
                obj->parent = ref_other.p;
            }
            ref_other.p = obj->other;
            *state = 0;
            load_palette_anim_from_table_index_slot27(obj);
        } else {
            obj->state_1 += 2;
        }
    }
}
