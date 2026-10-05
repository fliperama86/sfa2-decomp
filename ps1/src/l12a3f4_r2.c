/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8012f408(Object *object);
u16 func_80130470(Object *object);
u8 func_80130258(Object *object);
u8 func_8012f618(Object *object);
u8 func_8012efe4(Object *object);
u8 func_8012f898(Object *object);

void func_8012a684(Object *object) {
    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else if (func_8012f408(object)) {
        func_80130efc(object);
    } else if (object->field_cd != 0) {
        func_8012a804(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if (func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if (func_8012f618(object)) {
        func_8012f6a0(object);
    } else if (func_8012efe4(object)) {
        func_8013136c(object);
    } else if (func_8012f898(object)) {
        func_8012f8c4(object);
    } else {
        func_801301d8(object);
        func_80130efc(object);
    }
}
