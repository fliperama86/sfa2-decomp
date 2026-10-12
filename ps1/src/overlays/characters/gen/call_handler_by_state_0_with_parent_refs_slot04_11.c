/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c7ce0_slot04_11[];
extern ObjectRef data_80190458;
extern ObjectRef data_80190414;

void clear_object_and_push_on_free_stack(Object *o);
void set_position_from_ref_other_with_offset_slot04_11(Object *obj);
void follow_ref_other_and_advance_sequence_slot04_11(Object *obj);

void call_handler_by_state_0_with_parent_refs_slot04_11(Object *obj) {
    ref_other.p = obj->parent;
    data_80190458.p = ref_other.p->parent;
    data_80190414.p = obj->other;
    data_801c7ce0_slot04_11[obj->state_0](obj);
}

void init_object_sequence_1_at_other_slot04_11(Object *obj) {
    obj->field_01 = 1;
    obj->image_row = 2;
    obj->field_0c = 0;
    obj->field_20 = 0;
    obj->table_index_2 = 0;
    obj->ot_depth = 0;
    obj->state_0++;
    obj->field_1c = ref_other.p->field_1c;
    set_position_from_ref_other_with_offset_slot04_11(obj);
    set_sequence_from_table_5(obj, 1, seqs_8017c7f8);
    follow_ref_other_and_advance_sequence_slot04_11(obj);
}

void follow_ref_other_and_advance_sequence_slot04_11(Object *obj) {
    set_position_from_ref_other_with_offset_slot04_11(obj);
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    advance_sequence_checked(obj);
}

void unlink_from_four_slots_and_release_slot04_11(Object *o) {
    Object **p = (Object **)ref_other.p->parent;
    int i;

    i = 4;
    do {
        if (p == (Object **)o) {
            *(u32 *)o = 0;
            break;
        }
        p++;
    } while (--i != 0);
    clear_object_and_push_on_free_stack(o);
}

void set_position_from_ref_other_with_offset_slot04_11(Object *obj) {
    obj->pos_x = (u16)ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y + 0x10;
    if (obj->table_index != 0) {
        obj->pos_x = obj->pos_x - 0xc;
        obj->pos_y = obj->pos_y - 6;
    }
}
