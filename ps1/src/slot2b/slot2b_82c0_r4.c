/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef30_slot2b;
void increment_state_0_slot2b(Object *o);
void set_variant_and_sequence_from_table_slot2b(Object *o, u8 a);

void func_80078654_slot2b(Object *o) {
    Object *p;
    u8 t;

    o->field_01 = 0;
    p = data_8007ef30_slot2b;
    if (o->table_index != p->kind) {
        increment_state_0_slot2b(o);
    } else {
        t = p->frame->frame_param;
        if (t == 0) {
            o->variant_index = 0;
        } else {
            o->pos_x = p->pos_x;
            o->pos_y = p->pos_y;
            o->facing_side = p->facing_side;
            o->field_0c = data_8007ef30_slot2b->field_0c;
            o->image_row = data_8007ef30_slot2b->image_row;
            o->field_01 = 1;
            if (o->variant_index != t) {
                set_variant_and_sequence_from_table_slot2b(o, t);
            } else {
                advance_sequence_checked(o);
            }
        }
    }
}

void increment_state_0_slot2b(Object *o) {
    o->state_0++;
}

void unlink_object_and_call_release_slot2b(Object *o) {
    Slot2bObj *p = (Slot2bObj *)data_8007ef30_slot2b;

    if ((u32)o == p->linked_object) {
        p->linked_object = 0;
    }
    func_8011f38c(o);
}

void clear_variant_index_slot2b(Object *o) {
    o->variant_index = 0;
}
