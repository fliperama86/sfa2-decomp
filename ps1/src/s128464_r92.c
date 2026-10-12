/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80129b68(Object *object) {
    if (object->field_292 != 0) object->field_292--;
}

void func_80129b90(Object *object) {
    if (object->countdown_4 != 0) object->countdown_4--;
}

void func_80129bb8(Object *object) {
    if (object->other_lock != 0) object->other_lock--;
}

void func_80129be0(Object *object) {
    if (object->countdown_4 != 0) object->countdown_4--;
}
