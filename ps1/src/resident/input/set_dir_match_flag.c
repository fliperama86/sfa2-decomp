/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void set_dir_match_flag(Object *object, BytePair dir) {
    object->dir_match_flag = 0;
    if (object->field_25f != 0 && object->dir_first == dir.first && object->dir_second == dir.second &&
        object->field_17e == object->input_dir_0) {
        object->dir_match_flag = 1;
    }
}
