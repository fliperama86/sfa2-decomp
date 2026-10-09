/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013ebf4(Object *object, int index, int arg) {
    Slot *slot;
    object->slots[(u8)index].field_00++;
    arg = 0x20;
    if (object->field_7e != 0) arg = 5;
    object->slots[(u8)index].field_04 = arg;
    slot = &object->slots[(u8)index];
    slot->field_02 = 0;
    func_8013f2c8(object);
}
