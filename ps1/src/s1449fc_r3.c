/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80144dcc(Object *object) {
    int v;
    func_80144220(object);
    v = object->field_46 - 1;
    object->field_46 = v;
    if (v & 0x80) {
        func_801449fc(object);
    }
}
