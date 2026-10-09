/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8011a55c(Object *o);

void func_801490dc(Object *object) {
    table_8017cef0[object->field_04](object);
}

void func_8014911c(Object *object) {
    object->field_09 = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->field_04 = object->field_04 + 1;
    func_8011a55c(object);
}

void func_80149158(Object *object) {
    s16 old = object->field_46;
    object->field_46 = old - 1;
    if (old < 0) {
        /* Signed store: a plain assignment compiles to ori 0xffff. */
        *(s16 *)&object->field_46 = -1;
        if (object->field_3c->field_7e == 0) {
            object->field_04 = object->field_04 + 1;
        }
        func_8011ffdc(object);
    }
}
