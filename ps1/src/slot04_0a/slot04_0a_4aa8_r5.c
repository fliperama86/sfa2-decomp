/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void release_object_record_2_slot04_0a(Object *obj);

void copy_parent_position_start_side_table_sequence_slot04_0a(Object *obj) {
    Object *p;
    SequenceStep **t;
    obj->state_0++;
    p = obj->parent;
    obj->pos_x = p->pos_x;
    *(u16 *)&obj->pos_y = ((Slot04aObj *)p)->saved_pos_y;
    obj->field_1c = p->field_1c;
    obj->field_0e = p->field_0e;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->facing_side = p->facing_side;
    if (p->side == 0) {
        t = data_1f8000b4;
    } else {
        t = data_1f800164;
    }
    set_sequence_from_table_5(obj, 0x15, t);
}

void release_or_advance_by_parent_state_2_slot04_0a(Object *obj) {
    if (obj->parent->state_2 >= 2) {
        release_object_record_2_slot04_0a(obj);
    } else {
        advance_sequence_checked(obj);
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}
