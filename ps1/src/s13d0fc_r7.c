/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, u8 index, u8 arg);
void func_8013f2d8(Object *object, u8 index, u8 arg);

u8 func_8013e3b4(Object *object, u8 index, u8 arg) {
    table_8017abec[object->slots[index].field_00](object, index, arg);
    return data_80188f44;
}

void func_8013e408(Object *object, u8 index, u8 arg) {
    u16 entry;
    int mask;

    object->slots[index].field_01 = 0;
    entry = table_8017a8cc[arg * 7];
    mask = entry & 0xf0ff;
    if ((mask & object->field_134) == 0) {
        func_8013f2a8(object, index, arg);
    } else {
        object->slots[index].field_00++;
        if (entry & 1) {
            func_8013f2d8(object, index, arg);
        } else {
            func_8013f0c8(object);
        }
    }
}
