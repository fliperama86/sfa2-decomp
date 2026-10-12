/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void count_down_object_byte_counter(Object *object) {
    if (object->field_292 != 0) object->field_292--;
}

void count_down_countdown_4(Object *object) {
    if (object->countdown_4 != 0) object->countdown_4--;
}

void count_down_other_lock(Object *object) {
    if (object->other_lock != 0) object->other_lock--;
}

void count_down_countdown_4_2(Object *object) {
    if (object->countdown_4 != 0) object->countdown_4--;
}
