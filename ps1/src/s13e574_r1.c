/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013e574(Object *object, int index, u8 arg) {
    table_8017abf4[object->slots[(u8)index].stage]((Object *)object, (u8)index, arg);
    return data_80188f44;
}

void func_8013e5c8(Object *object, int index, int arg) {
    int v;
    object->slots[(u8)index].entry_index = 0;
    object->slots[(u8)index].stage = object->slots[(u8)index].stage + 1;
    v = table_8017aaf8[(u8)arg * 7];
    if (object->field_7e != 0) v = 5;
    object->slots[(u8)index].timer = v;
    func_8013e634(object, (u8)index, (u8)arg);
}
