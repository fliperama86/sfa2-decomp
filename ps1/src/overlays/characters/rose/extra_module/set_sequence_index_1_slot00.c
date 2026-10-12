/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void set_sequence_by_sound_side_slot00(Object *obj, int mode);

void set_sequence_index_1_slot00(Object *obj) {
    set_sequence_by_sound_side_slot00(obj, 1);
}

void init_object_at_parent_offset_sequence_3_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->facing_side = p->facing_side;
    if (obj->facing_side != 0) {
        obj->pos_x = (u16)p->pos_x - 7;
    } else {
        obj->pos_x = (u16)p->pos_x + 7;
    }
    obj->pos_y = (u16)p->pos_y + 5;
    obj->variant_index = (get_random_byte() & 0x1f) + 0xa;
    set_sequence_by_sound_side_slot00(obj, 3);
}

void set_random_sequence_index_4_to_7_slot00(Object *obj) {
    set_sequence_by_sound_side_slot00(obj, (get_random_byte() & 3) | 4);
}
