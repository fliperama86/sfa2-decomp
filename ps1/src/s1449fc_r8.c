/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011a55c(void);

void func_80145be0(void) {
    func_8011f240();
}

void func_80145c00(Object *object) {
    table_8017c9d0[object->field_04](object);
}

void func_80145c40(Object *object) {
    object->field_09 = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->field_04++;
    func_8011a55c();
}

void func_80145c7c(Object *object) {
    int v;
    int old = object->field_46;
    object->field_46 = old - 1;
    if ((s16)old < 0) {
        v = -1;
        object->field_46 = v;
        if (object->field_50 != (object->field_3c->field_04 << 24) + (object->field_3c->field_05 << 16) + (object->field_3c->field_06 << 8)) {
            object->field_04++;
        }
        func_8011ffdc(object);
    }
}

void func_80145d00(void) {
    func_8011f240();
}
