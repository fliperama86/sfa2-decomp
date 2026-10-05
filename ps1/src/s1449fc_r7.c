/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80145aa0(void) {
    func_8011f240();
}

void func_80145ac0(Object *object) {
    table_8017c9c0[object->field_04](object);
}

void func_80145b00(Object *object) {
    if (object->field_03 == 3 || object->field_03 == 10) {
        func_8011f240();
    } else {
        object->field_01 = 0;
        object->field_04++;
        func_8011fc54(object, table_8017c988, object->field_03);
    }
}
