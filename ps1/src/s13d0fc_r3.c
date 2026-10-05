/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, u8 index, u8 arg);
void func_8013f2d8(Object *object, u8 index, u8 arg);

void func_8013d358(Object *object, u8 index, u8 arg) {
    u16 entry;

    object->slots[index].field_04--;
    if (object->slots[index].field_04 == 0) {
        func_8013f2a8(object, index, arg);
    } else {
        entry = table_8017a8cc[arg * 7 + object->slots[index].field_01];
        if ((entry & 0xf000) != (object->field_150 & 0xf000)) {
            func_8013f2c8(object, index, arg);
        } else {
            if (entry & 1) {
                object->slots[index].field_00++;
            }
            func_8013f0c8(object, index, arg);
        }
    }
}

void func_8013d418(Object *object, u8 index, u8 arg) {
    if (object->field_7e == 0) {
        object->slots[index].field_04--;
        if (object->slots[index].field_04 == 0) {
            func_8013f2a8(object, index, arg);
        }
    }
    if ((object->field_134 | object->field_136) & 0x100) {
        func_8013f2d8(object, index, arg);
    } else {
        func_8013f2c8(object);
    }
}

u8 func_8013d4cc(Object *object, u8 index, u8 arg) {
    table_8017abc4[object->slots[index].field_00](object, index, arg);
    return data_80188f44;
}
