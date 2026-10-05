/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80130678(Object *object, int arg);

void func_8012e888(Object *object) {
    if (object->side == 0) {
        scratch_call_5c(object);
    } else {
        scratch_call_10c(object);
    }
}

void func_8012e8d4(Object *object) {
    if (object->kind == 2 || object->kind == 6 || object->kind == 0xc ||
        object->kind == 0xe || object->kind == 0x11 ||
        (u8)(object->kind - 0x13) < 2) {
        if (object->side == 0) {
            scratch_call_98(object);
        } else {
            scratch_call_148(object);
        }
    } else {
        func_8012e984(object);
    }
}

void func_8012e984(Object *object) {
    handlers_1a80[object->field_06](object);
}

void func_8012e9c4(Object *object) {
    object->field_06 = object->field_06 + 1;
    object->field_0b = object->field_158;
    func_80130678(object, 0);
}

void func_8012e9f8(Object *object) {
    if (game_state.config->field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}

void func_8012ea40(Object *object) {
    int arg = 0x28;

    object->field_46 = 0x78;
    object->field_06 = object->field_06 + 1;
    if (game_state.config->field_a6 != 0) {
        arg = 0x29;
    }
    func_80130678(object, arg);
}
