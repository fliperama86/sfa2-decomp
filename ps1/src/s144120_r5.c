/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144734(Object *object) {
    func_80144220(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        func_80144e90(object);
    }
}

void func_80144780(Object *object) {
    handlers_b630[object->state_1](object);
}

void func_801447c0(Object *object) {
    handlers_b640[object->state_2](object);
    advance_sequence_checked(object);
    if (object->state_2 >= 2) {
        func_80120028(object);
    }
}
