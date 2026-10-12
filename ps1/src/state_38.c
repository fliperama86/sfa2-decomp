/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80138c78(GameState *state, Object *object) {
    func_80138ae8(state, object);
}

void func_80138c98(Object *object) {
    Object *pair = &player_left;

    func_80138ce0(object, pair);
    func_80138ce0(object, pair + 1);
}

void func_80138ce0(Object *object, Object *other) {
    int a;
    int b;

    if (object->field_49 >= 0x50) {
        a = 0;
    } else if (object->field_49 >= 0x3c) {
        a = 2;
    } else if (object->field_49 >= 0x28) {
        a = 4;
    } else {
        a = 6;
    }
    if ((short)other->health >= 0x70) {
        b = 0;
    } else if ((short)other->health >= 0x48) {
        b = 2;
    } else if ((short)other->health >= 0x20) {
        b = 4;
    } else {
        b = 6;
    }
    if (b >= a) {
        other->script_table_index = b;
    } else {
        other->script_table_index = a;
    }
}
