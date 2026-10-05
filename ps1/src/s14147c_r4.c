/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80142718(Object *object) {
    if (object->field_4b != 0) {
        func_80142940(object);
    } else {
        u16 v = object->field_134;
        game_state.field_354 = v;
        game_state.field_354 = (v | object->field_136) & 0x95;
        func_801427d8(object);
    }
}

void func_80142778(Object *object) {
    if (object->field_4b != 0) {
        func_80142940(object);
    } else {
        u16 v = object->field_134;
        game_state.field_354 = v;
        game_state.field_354 = (v | object->field_136) & 0x6a;
        func_801427d8(object);
    }
}
