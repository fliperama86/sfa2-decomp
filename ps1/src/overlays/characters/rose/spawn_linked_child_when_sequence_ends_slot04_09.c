/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_linked_child_when_sequence_ends_slot04_09(Object *o, Object *unused) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p;
    Object *q;

    if ((s16)o->step_flags & 0x8000) {
        p = (Object *)pop_block_from_pool();
        if (p == 0) {
            return;
        }
        p->active_count = 1;
        p->field_02 = 0xc;
        p->table_index = 0xc;
        p->field_0e = o->field_0e;
        p->field_0c = o->field_0c;
        p->image_row = o->image_row;
        p->field_1c = o->field_1c;
        p->scroll_factor = o->scroll_factor;
        p->parent = o;
        p->mode_id = 0x20;
        p->field_90 = o->field_90;
        p->data_ptr_a = o->data_ptr_a;
        p->data_ptr_b = o->data_ptr_b;
        p->spawn_x = o->spawn_x;
        p->spawn_y = o->spawn_y;
        p->sound_side = o->side;
        o->gauge = 0x3c;
        obj->linked_object = (u32)p;
        o->countdown = 0;
        o->state_3++;
        q = o->other;
        obj->field_334 = q->field_0c;
        obj->field_335 = q->image_row;
        play_sound_at(o, o->side ^ 1, 0x31a);
        play_side_sound_at(o, o->side, 9);
        play_side_sound_at(o, o->side, 0x10);
        set_sequence_by_side_and_mode(o, 0x19);
    } else {
        update_sequence(o);
    }
    set_position_from_other_box(o);
}
