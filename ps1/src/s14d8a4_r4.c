/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014e080(Object *object) {
    object->script_mode = 2;
    object->init_done_flag = 0;
    object->field_207 = 0;
    object->field_208 = 0;
    object->field_209 = 0;
    func_8014a1ec(object);
}
