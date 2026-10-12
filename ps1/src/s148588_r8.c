/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_801490dc(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cef0[object->state_0](object);
}

void func_8014911c(Object *object) {
    object->ot_depth = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->state_0 = object->state_0 + 1;
    func_8011a55c(object);
}

void func_80149158(Object *object) {
    s16 old = object->countdown;
    object->countdown = old - 1;
    if (old < 0) {
        /* Signed store: a plain assignment compiles to ori 0xffff. */
        *(s16 *)&object->countdown = -1;
        if (object->parent->field_7e == 0) {
            object->state_0 = object->state_0 + 1;
        }
        func_8011ffdc(object);
    }
}
