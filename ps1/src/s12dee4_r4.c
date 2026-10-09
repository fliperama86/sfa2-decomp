/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8014cbf8(Object *object);

void func_8012e4c4(Object *object) {
    object->field_207 = 0;
    object->field_208 = 0;
    object->field_209 = 0;
    if (object->field_20c != 0) {
        func_8014cbf8(object);
    }
    if (func_80149d48(object)) {
        func_80149fc4(object);
    } else if (object->field_157 == 0) {
        func_801312b8(object);
    } else {
        func_80131468(object);
    }
}

void func_8012e550(Object *object) {
    if (object->side == 0) {
        if (object->field_cd == 0) {
            scratch_call_8c(object);
        } else {
            scratch_call_e4(object);
        }
    } else {
        if (object->field_cd == 0) {
            scratch_call_13c(object);
        } else {
            scratch_call_194(object);
        }
    }
    if (object->field_73 == 0) {
        func_80130b10(object);
    }
}
