/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void follow_parent_step_flags_for_sequence_slot04_02(Object *obj);
void increment_state_0_2_slot04_02(Object *obj);
void set_sequence_from_variant_index_slot04_02(Object *obj);
void copy_parent_position_with_facing_offset_slot04_02(Object *obj);

void init_object_from_parent_fields_2_slot04_02(Object *obj) {
    Object *p = obj->parent;

    obj->state_0++;
    obj->field_1c = p->field_1c;
    obj->table_index = p->kind;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->spawn_x = 0x60;
    obj->variant_index = 0;
    obj->spawn_y = 0x1e0;
    obj->field_90 = p->field_90;
    obj->data_ptr_a = p->data_ptr_a;
    obj->data_ptr_b = p->data_ptr_b;
    follow_parent_step_flags_for_sequence_slot04_02(obj);
}

void follow_parent_step_flags_for_sequence_slot04_02(Object *obj) {
    Object *p = obj->parent;
    u16 t;
    u16 u;
    u16 w;

    obj->field_01 = 0;
    if (obj->table_index != p->kind) {
        increment_state_0_2_slot04_02(obj);
    } else {
        w = p->step_flags;
        t = w & 0xff;
        u = t;
        if (u == 0) {
            obj->variant_index = 0;
        } else if ((w & 0x80) != 0) {
            increment_state_0_2_slot04_02(obj);
        } else {
            copy_parent_position_with_facing_offset_slot04_02(obj);
            obj->field_01 = 1;
            if (obj->variant_index != u) {
                obj->variant_index = t;
                set_sequence_from_variant_index_slot04_02(obj);
            } else {
                advance_sequence_checked(obj);
                clear_or_set_flag_by_pos_x_limit(obj);
            }
        }
    }
}
