/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012ce5c(Object *object) {
    table_80171a08[object->field_60](object);
}

void func_8012ce9c(Object *object) {
    table_80171a20[object->field_07](object);
}

void func_8012cedc(Object *object) {
    object->field_07 = object->field_07 + 1;
    func_8012d0bc(object);
    func_8012cf1c(object);
}
