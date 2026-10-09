/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144734(Object *object) {
    func_80144220(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) {
        func_80144e90(object);
    }
}

void func_80144780(Object *object) {
    handlers_b630[object->field_05](object);
}

void func_801447c0(Object *object) {
    handlers_b640[object->field_06](object);
    func_80131094(object);
    if (object->field_06 >= 2) {
        func_80120028(object);
    }
}
