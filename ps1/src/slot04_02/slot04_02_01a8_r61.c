/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void increment_state_0_slot04_02(Object *obj);
void set_sequence_from_variant_index_by_parent_side_slot04_02(Object *obj);
void copy_parent_position_offset_by_facing_slot04_02(Object *obj);

void init_object_from_parent_fields_slot04_02(Object *o) {
    Object *p = o->parent;
    o->state_0++;
    o->field_1c = p->field_1c;
    o->table_index = p->kind;
    o->field_0c = p->field_0c;
    o->image_row = p->image_row;
    o->field_0e = p->field_0e;
    o->spawn_x = 0x60;
    o->variant_index = 0;
    o->spawn_y = 0x1e0;
    o->ot_depth = 0;
}

void update_child_follow_parent_frame_param_slot04_02(Object *o) {
    Object *p = o->parent;
    int s;
    o->field_01 = 0;
    if (o->table_index != p->kind) {
        increment_state_0_slot04_02(o);
    } else {
        s = p->frame->frame_param;
        if (s == 0) {
            o->variant_index = 0;
        } else {
            copy_parent_position_offset_by_facing_slot04_02(o);
            o->field_01 = 1;
            if (o->variant_index != s) {
                o->variant_index = s;
                set_sequence_from_variant_index_by_parent_side_slot04_02(o);
            } else {
                advance_sequence_checked(o);
            }
        }
    }
}
