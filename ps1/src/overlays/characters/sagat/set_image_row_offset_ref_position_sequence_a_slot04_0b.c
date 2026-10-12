/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_from_other_side_table_slot04_0b(Object *obj, int index);
void set_position_relative_to_ref_other_slot04_0b(Object *obj, int dx, int dy);

void set_image_row_offset_ref_position_sequence_a_slot04_0b(Object *obj) {
    obj->image_row = 0x10;
    obj->state_1++;
    set_position_relative_to_ref_other_slot04_0b(obj, 6, 0);
    set_sequence_from_other_side_table_slot04_0b(obj, 0x16);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void spawn_pooled_object_on_ref_step_flag_slot04_0b(Object *obj) {
    Object *n;
    if (ref_other.p->step_flags & 1) {
        obj->state_1++;
        set_position_relative_to_ref_other_slot04_0b(obj, -0x19, -3);
        set_sequence_from_other_side_table_slot04_0b(obj, 0x17);
        n = (Object *)pop_block_from_pool();
        if (n != 0) {
            n->active_count = 1;
            n->field_02 = 6;
            n->table_index = 2;
            n->parent = obj->parent;
            n->sound_side = ((Slot04aObj *)ref_other.p)->side;
            n->mode_id = 0x20;
            n->field_90 = ref_other.p->field_90;
            n->data_ptr_a = ref_other.p->data_ptr_a;
            n->data_ptr_b = ref_other.p->data_ptr_b;
            if (n->sound_side == 0) {
                n->spawn_x = 0x100;
            } else {
                n->spawn_x = 0x120;
            }
            n->spawn_y = 0x1e0;
        }
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}
