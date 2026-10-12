/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013ee80(Object *object, int index, u8 arg) {
    table_8017ac14[object->slots[(u8)index].stage](object, (u8)index, arg);
    return data_80188f44;
}

void func_8013eed4(Object *object, int index, int arg) {
    u8 v = object->slots[(u8)index].stage;
    object->slots[(u8)index].entry_index = 1;
    object->slots[(u8)index].repeat_count = 0;
    object->slots[(u8)index].direction = 1;
    object->slots[(u8)index].timer = 0;
    object->slots[(u8)index].level = 1;
    object->slots[(u8)index].field_06 = 0;
    object->slots[(u8)index].stage = v + 1;
    func_8013ef28(object, (u8)index, (u8)arg);
}
