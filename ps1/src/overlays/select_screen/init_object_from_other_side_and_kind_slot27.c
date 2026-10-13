/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_800268b0_slot27[];
void copy_side_palette_from_table_and_upload_slot27(Object *obj);
void set_position_from_table_by_other_side_slot27(Object *obj);

void init_object_from_other_side_and_kind_slot27(Object *obj) {
    Object *p;
    u32 *g;
    int d;
    int v;

    obj->ot_depth = 0xc;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->field_90 = (void *)0x80038000;
    p = obj->other;
    obj->spawn_x = 0;
    obj->image_row = 0;
    obj->field_01 = 0;
    obj->state_1 = obj->state_1 + 1;
    g = (u32 *)scratch_word_b;
    v = p->side;
    *g = v;
    d = 2;
    if (v != 0) {
        d = 5;
    }
    obj->image_row = d;
    obj->facing_side = *(u8 *)g;
    d = p->kind;
    obj->table_index = d;
    obj->variant_index = d;
    set_position_from_table_by_other_side_slot27(obj);
    copy_side_palette_from_table_and_upload_slot27(obj);
}

void start_sequence_if_side_bit_set_slot27(Object *obj) {
    Object *p = obj->other;

    if (((game_state.mode | game_state.side_mask) >> p->side) & 1) {
        obj->state_1 = 0;
        obj->state_0 = obj->state_0 + 1;
        set_sequence_from_table_5(obj, obj->variant_index, data_800268b0_slot27);
        obj->field_01 = 1;
    }
}
