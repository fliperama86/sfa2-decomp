/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80144dcc(Object *object) {
    int v;
    func_80144220(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        func_801449fc(object);
    }
}
