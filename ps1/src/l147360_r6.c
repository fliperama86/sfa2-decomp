/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014a1ec(Object *object) {
    if (game_state.field_4e == 0) {
        if (object->field_21c >= 0x100) object->field_21c = 0xff;
        if (object->field_20a == 0) {
            if (object->field_206 == 0) {
                object->field_206 = object->field_206 + 1;
                func_8014d9ac(object);
            } else if (object->field_206 != 1) return;
            func_8014a364(object);
        } else if (object->field_20a == 1) {
            if (object->field_206 == 0) {
                object->field_206 = object->field_206 + 1;
                func_8014da18(object);
            } else if (object->field_206 != 1) return;
            func_8014a3c8(object);
        } else if (object->field_20a == 2) {
            if (object->field_206 == 0) {
                object->field_206 = object->field_206 + 1;
                func_8014dae8(object);
            } else if (object->field_206 != 1) return;
            func_8014a42c(object);
        } else if (object->field_20a == 3) {
            if (object->field_206 == 0) {
                object->field_206 = object->field_206 + 1;
                func_8014dbc8(object);
            } else if (object->field_206 != 1) return;
            func_8014a490(object);
        }
    }
}
