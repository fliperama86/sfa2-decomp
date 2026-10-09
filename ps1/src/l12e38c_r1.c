/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012e38c(Object *object) {
    func_80155f30(object);
    object->field_69 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        u8 count;

        count = object->field_1a1 - 1;
        object->field_1a1 = count;
        if (count & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_170 = 0;
    object->field_6b = 0;
    object->field_28a = 0;
    object->field_288 = 0;
    object->field_289 = 0;
    func_8012f5ec(object);
    object->field_249 = 5;
    object->field_0b = object->field_158;
    if (object->field_cd != 0) {
        func_8012e4c4(object);
    } else {
        object->field_25e = 2;
        if (func_80130470(object) != 0) {
            func_8013047c(object);
        } else if (func_8012f970(object) != 0) {
            func_8012fe60(object);
        } else if (object->field_130 & 0x4000) {
            func_80131468(object);
        } else {
            func_801312b8(object);
        }
    }
}
