/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013ed1c(Object *object, int index) {
    Slot *slot = &object->slots[(u8)index];
    (*(u16 *)&slot->field_02)++;
    object->slots[(u8)index].field_04--;
    if (object->slots[(u8)index].field_04 == 0) object->slots[(u8)index].field_00++;
    func_8013f2c8(object);
}
