/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void init_object_mode_3(Object *object) {
    object->script_mode = 3;
    object->init_done_flag = 0;
    object->field_207 = 0;
    object->field_208 = 0;
    object->field_209 = 0;
    func_8014a1ec(object);
}
