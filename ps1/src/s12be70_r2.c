/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012c06c(Object *object) {
    object->field_16a = 0;
    table_801719f4[object->field_06](object);
    func_80130b10(object);
}

void func_8012c0c0(Object *object) {
    table_80171a00[object->field_07](object);
}
