/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801428a8(Object *object) {
    int v;
    int a = *(s16 *)&object->meter;
    object->field_299 = 1;
    v = 0;
    if (a >= 0x60) {
        v = 2;
        if (a >= 0x90) v = 6;
    }
    object->direction_index = v;
    object->field_255 = v;
    object->saved_meter = object->meter;
}
