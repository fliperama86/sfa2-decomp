/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_child_index_6_and_update_sequence_slot04_06(Object *obj) {
    Object *p;

    if (((Slot04aObj *)obj)->status_flags & 0x80) {
        game_state_second.side_bits = game_state.side_bits_2 | (1 << obj->side);
    }
    if (((Slot04aObj *)obj)->mode != 0) {
        ((Slot04aObj *)obj)->mode = 0;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->table_index = 6;
            p->parent = obj;
            p->field_1c = obj->field_1c;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->image_row = obj->image_row;
            p->field_02 = 0x11;
            p->mode_id = 0x20;
            p->field_90 = obj->field_90;
            p->sound_side = obj->sound_side;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
    update_sequence(obj);
}
