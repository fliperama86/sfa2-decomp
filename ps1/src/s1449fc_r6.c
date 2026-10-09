/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014594c(Block172 *block) {
    Object *object = (Object *)block;
    table_8017c978[object->field_04](object);
}

void func_8014598c(Object *object) {
    object->field_01 = 0;
    object->field_04++;
    if (object->field_0e == 4) {
        func_8011f8e4(object, table_8017c90c, object->field_03);
    } else if (object->field_0e == 8) {
        func_8011f93c(object, table_8017c910, object->field_03);
    } else if (object->field_0e == 0xc) {
        func_8011fa20(object, table_8017c960, object->field_03);
    }
}
