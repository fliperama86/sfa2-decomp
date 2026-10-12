/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80142718(Object *object) {
    if (object->field_4b != 0) {
        func_80142940(object);
    } else {
        u16 v = object->pressed_bits;
        game_state.field_354 = v;
        game_state.field_354 = (v | object->released_bits) & 0x95;
        set_direction_index_from_meter_limit(object);
    }
}

void func_80142778(Object *object) {
    if (object->field_4b != 0) {
        func_80142940(object);
    } else {
        u16 v = object->pressed_bits;
        game_state.field_354 = v;
        game_state.field_354 = (v | object->released_bits) & 0x6a;
        set_direction_index_from_meter_limit(object);
    }
}
