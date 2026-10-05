/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80123d0c(Ref40 *source, Object *object) {
    if (object->field_cd == 0) {
        object->field_11b = source->field_40;
        object->field_c0 = 0;
        object->field_e8 = func_80156018(object->field_e8, object->field_e4);
        object->field_e4 = object->field_b8;
    }
}
