/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80123d0c(Ref40 *source, Object *object) {
    if (object->mode_flag_2 == 0) {
        object->field_11b = source->copied_value;
        object->count = 0;
        object->best_score = func_80156018(object->best_score, object->score_2);
        object->score_2 = object->score;
    }
}
