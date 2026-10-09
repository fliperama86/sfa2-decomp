/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, int index, int unused);
void func_8013f2d8(Object *object, int index, int unused);

void func_8013df68(Object *object, u8 index, u8 arg) {
    u16 entry;

    object->slots[index].field_04--;
    if (object->slots[index].field_04 == 0) {
        func_8013f2a8(object, index, arg);
    } else {
        entry = table_8017a8cc[arg * 7 + object->slots[index].field_01];
        if ((entry & 0xf000) != (object->field_150 & 0xf000)) {
            func_8013f2c8(object);
        } else {
            if (entry & 1) {
                object->slots[index].field_00++;
            }
            func_8013f0c8(object, index, arg);
        }
    }
}

