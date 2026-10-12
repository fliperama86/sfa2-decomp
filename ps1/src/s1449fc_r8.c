/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8017c9c0 holds this function and is declared with this parameter. */
void func_80145be0(Object *p) {
    func_8011f240((Slab172 *)p);
}

void func_80145c00(Block172 *block) {
    Object *object = (Object *)block;
    table_8017c9d0[object->state_0](object);
}

void func_80145c40(Object *object) {
    object->ot_depth = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->state_0++;
    func_8011a55c(object);
}

void func_80145c7c(Object *object) {
    int v;
    int old = object->countdown;
    object->countdown = old - 1;
    if ((s16)old < 0) {
        v = -1;
        object->countdown = v;
        if (object->vel_y != (object->parent->state_0 << 24) + (object->parent->state_1 << 16) + (object->parent->state_2 << 8)) {
            object->state_0++;
        }
        func_8011ffdc(object);
    }
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8017c9d0 holds this function and is declared with this parameter. */
void func_80145d00(Object *p) {
    func_8011f240((Slab172 *)p);
}
