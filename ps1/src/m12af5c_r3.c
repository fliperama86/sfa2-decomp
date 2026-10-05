/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012bf74(Object *object) {
    func_80155f30(object);
    func_80120554(object, object->side, 0x323);
    object->field_69 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if ((object->field_1a1 & 0x80) != 0) {
            object->field_19f = 0;
        }
    }
    object->field_0b = object->field_158;
    object->field_170 = 0;
    object->field_292 = 0x20;
    if (object->field_cd == 0) {
        if (func_8012f970(object) != 0) {
            func_8012fe60(object);
        } else if ((object->field_130 & 0x4000) != 0) {
            func_80131468(object);
        } else {
            func_801312b8(object);
        }
    } else {
        func_801312b8(object);
    }
}
