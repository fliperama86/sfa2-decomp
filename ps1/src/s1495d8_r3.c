/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80149aa8(Object *object) {
    object->state_0 = 2;
    object->field_01 = 0;
}

void func_80149ab8(Object *object) {
    play_sound_at(object->parent, object->parent->side, 0x329);
    func_8011f240(object);
}

void func_80149af8(Object *object) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->active_count++;
        ref_other.p->field_02 = 0x6d;
        ref_other.p->table_index = object->side;
        ref_other.p->parent = object;
    }
}
