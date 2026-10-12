/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_direction_index_from_buttons_mask_a(Object *object) {
    if (object->field_4b != 0) {
        set_direction_index_four_then_update_meter(object);
    } else {
        u16 v = object->pressed_bits;
        game_state.field_354 = v;
        game_state.field_354 = (v | object->released_bits) & 0x95;
        set_direction_index_from_meter_limit(object);
    }
}

void set_direction_index_from_buttons_mask_b(Object *object) {
    if (object->field_4b != 0) {
        set_direction_index_four_then_update_meter(object);
    } else {
        u16 v = object->pressed_bits;
        game_state.field_354 = v;
        game_state.field_354 = (v | object->released_bits) & 0x6a;
        set_direction_index_from_meter_limit(object);
    }
}
