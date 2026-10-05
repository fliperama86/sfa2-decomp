/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013f2a8(Object *object, int index, int arg);

void func_8013e7f0(Object *object, int index, u8 arg) {
    object->slots[(u8)index].field_04--;
    if (object->slots[(u8)index].field_04 == 0) func_8013f2a8(object, (u8)index, arg);
    else func_8013e84c(object, (u8)index, arg);
}
