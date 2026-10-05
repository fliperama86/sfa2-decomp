/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014147c(Object *object, BytePair dir) {
    object->field_279 = 0;
    if (object->field_25f != 0 && object->field_17d == dir.first && object->field_17c == dir.second &&
        object->field_17e == object->field_128) {
        object->field_279 = 1;
    }
}
