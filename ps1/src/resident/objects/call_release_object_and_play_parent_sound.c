/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_release_object_and_play_parent_sound(Object *object) {
    release_object_and_play_parent_sound(object);
}

void release_object_and_play_parent_sound(Object *object) {
    Object *other;
    if (object->active_state != 0) {
        other = object->parent;
        other->field_29e = 0;
        play_sound_at(other, other->side, 0x323);
    }
    reset_and_push_object_to_pool_table(object);
}
