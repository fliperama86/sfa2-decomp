/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80147238(Object *object) {
    u8 phase;
    u8 key;
    data_80189458 = 0;
    data_8018945c = 0;
    phase = object->field_04;
    object->field_04 = phase + 1;
    if (game_state.field_64 != 0) {
        func_80120554(0, 0, 0x32c);
        func_8014f604(2);
        object->field_01 = 1;
        func_801476f8(object);
        func_80130768(object, game_state.field_6b, table_8017cbac);
        key = object->field_45 >> 1;
        if (key == 0) {
            bytes_8017cb18.first = 0;
            bytes_8017cb18.second = 0;
        } else if (key == 1) {
            bytes_8017cb18.first = 0;
            bytes_8017cb18.second = 0;
        } else if (key == 2) {
            bytes_8017cb18.first = 0;
            bytes_8017cb18.second = 0x10;
        } else if (key == 3) {
            bytes_8017cb18.first = 1;
            bytes_8017cb18.second = 0x10;
        }
        func_801477ac(object, bytes_8017cb18.first, bytes_8017cb18.second);
    } else {
        object->field_04 = phase + 3;
    }
}
