/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013ee80(Object *object, int index, u8 arg) {
    table_8017ac14[object->slots[(u8)index].field_00](object, (u8)index, arg);
    return data_80188f44;
}

void func_8013eed4(Object *object, int index, int arg) {
    u8 v = object->slots[(u8)index].field_00;
    object->slots[(u8)index].field_01 = 1;
    object->slots[(u8)index].field_02 = 0;
    object->slots[(u8)index].field_03 = 1;
    object->slots[(u8)index].field_04 = 0;
    object->slots[(u8)index].field_05 = 1;
    object->slots[(u8)index].field_06 = 0;
    object->slots[(u8)index].field_00 = v + 1;
    func_8013ef28(object, (u8)index, (u8)arg);
}
