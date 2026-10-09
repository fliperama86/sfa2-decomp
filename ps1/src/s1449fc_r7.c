/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8017c978 holds this function and is declared with this parameter. */
void func_80145aa0(Object *p) {
    func_8011f240((Slab172 *)p);
}

void func_80145ac0(Object *object) {
    table_8017c9c0[object->field_04](object);
}

void func_80145b00(Object *object) {
    if (object->field_03 == 3 || object->field_03 == 10) {
        func_8011f240((Slab172 *)object);
    } else {
        object->field_01 = 0;
        object->field_04++;
        func_8011fc54(object, (SeqRec **)table_8017c988, object->field_03);
    }
}
