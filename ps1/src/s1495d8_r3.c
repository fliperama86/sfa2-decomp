/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80149aa8(Object *object) {
    object->state_0 = 2;
    object->field_01 = 0;
}

void play_parent_sound_and_release_object(Object *object) {
    play_sound_at(object->parent, object->parent->side, 0x329);
    reset_and_push_object_to_pool_table(object);
}

void spawn_object_type_6d_for_side(Object *object) {
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count++;
        ref_other.p->field_02 = 0x6d;
        ref_other.p->table_index = object->side;
        ref_other.p->parent = object;
    }
}
