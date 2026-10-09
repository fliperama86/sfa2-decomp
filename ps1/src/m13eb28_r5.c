/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f0c8(Object *object, int index, int unused) {
    Bytes32 table = data_8016d9ac;
    object->slots[(u8)index].field_01++;
    object->slots[(u8)index].field_04 = table.b[func_80151184() & 0x1f];
    func_8013f2c8(object);
}

void func_8013f1bc(Object *object, u8 index, u8 unused) {
    Bytes32 table = data_8016d9ac;
    object->slots[index].field_04 = table.b[func_80151184() & 0x1f];
    func_8013f2c8(object);
}
