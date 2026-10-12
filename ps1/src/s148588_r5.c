/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80148a04(Object *object) {
    func_80148a24(object);
}

void func_80148a24(Object *object) {
    Object *other;
    if (object->active_state != 0) {
        other = object->parent;
        other->field_29e = 0;
        play_sound_at(other, other->side, 0x323);
    }
    func_8011f240(object);
}
