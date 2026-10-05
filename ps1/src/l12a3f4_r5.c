/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80128778(void);
u16 func_80130470(Object *object);
u8 func_80130258(Object *object);
u8 func_8012f800(Object *object);
u8 func_8012f618(Object *object);
void func_80130678(Object *object, int index);

void func_8012b324(Object *object) {
    u16 saved;
    if (object->field_cd != 0) {
        func_8012b2c4(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if (func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if (func_8012f800(object)) {
        func_8012f838(object);
    } else if (!func_8012f618(object)) {
        saved = object->field_38;
        object->field_07 = 0;
        func_80130678(object, (u8)object->field_3a + 10);
        object->field_38 = saved;
    } else if ((s16)object->field_3a < 0) {
        func_80131468(object);
    } else {
        object->field_0b = object->field_158;
        func_80130efc(object);
    }
}

void func_8012b45c(Object *object) {
    if (object->field_7e != 0) {
        func_80128778();
    }
    func_80129be0(object);
    if (object->field_cd == 0) {
        if (object->field_296 != 0) {
            object->field_296--;
        }
        if (object->field_29b != 0) {
            object->field_29b--;
        }
        if (object->field_248 != 0) {
            object->field_248--;
        }
    }
    if (object->side == 0) {
        if (object->field_cd != 0) {
            scratch_call_d8(object);
        } else {
            scratch_call_54(object);
        }
    } else {
        if (object->field_cd != 0) {
            scratch_call_188(object);
        } else {
            scratch_call_104(object);
        }
    }
}
