/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, u8 index, u8 arg);

void func_8013de80(Object *object, u8 index, u8 arg) {
    u16 entry;
    u8 step;

    object->slots[index].field_01 = 0;
    entry = table_8017a8cc[arg * 7];
    if ((entry & 0xf000) != (object->field_150 & 0xf000)) {
        func_8013f2a8(object, index, arg);
    } else {
        step = object->slots[index].field_00;
        object->slots[index].field_00 = step + 1;
        if (!(entry & 1)) {
            func_8013f0c8(object, index, arg);
        } else {
            object->slots[index].field_00 = step + 2;
            func_8013f0c8(object, index, arg);
            func_8013e028(object, index, arg);
        }
    }
}
