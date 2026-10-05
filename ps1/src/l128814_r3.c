/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80128778(void);
void func_80130678(Object *object, int index);
u8 func_8012f408(Object *object);
u8 func_8012f898(Object *object);
u16 func_80130470(Object *object);
u8 func_8012f618(Object *object);
u8 func_80130258(Object *object);
u8 func_8012f7c0(Object *object);

void func_80129da8(Object *object) {
    object->field_157 = 0;
    if (object->field_7e != 0) {
        func_80128778();
        if (object->field_7e == 1) {
            object->field_225 = 0;
            func_80130678(object, 0);
        }
    }
    if (game_state.config->field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else if (func_8012f408(object) == 0 && game_state.config->field_4e == 0) {
        if (func_8012f898(object)) {
            func_8012f8c4(object);
        } else if (object->field_cd == 0) {
            if (func_80130470(object)) {
                func_8013047c(object);
            } else if (func_8012f618(object)) {
                func_8012f6a0(object);
            } else if (object->field_7e == 0) {
                if (func_80130258(object)) {
                    func_80130280(object);
                } else if (func_8012f970(object)) {
                    func_8012fe60(object);
                } else if (func_8012f7c0(object)) {
                    func_8012f838(object);
                } else {
                    func_80130efc(object);
                }
            } else {
                func_80130efc(object);
            }
        } else {
            func_80130efc(object);
        }
    } else {
        func_80130efc(object);
    }
}

void func_80129f80(Object *object) {
    object->field_157 = 1;
    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else if (func_8012f408(object) != 0) {
        func_8012a108(object);
    } else if (func_8012f898(object)) {
        func_8012f8c4(object);
    } else if (object->field_cd != 0) {
        func_8012a108(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if (func_8012f618(object)) {
        if (func_80130258(object)) {
            func_80130280(object);
        } else if (func_8012f970(object)) {
            func_8012fe60(object);
        } else {
            func_8012a108(object);
        }
    } else if (object->field_7e == 0 && func_8012f7c0(object)) {
        func_8012f838(object);
    } else {
        func_8012f784(object);
    }
}
