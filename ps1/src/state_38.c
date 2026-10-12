/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_add_one_to_ec4_wrapper_3(GameState *state, Object *object) {
    add_one_to_ec4_if_object_unflagged(state, object);
}

void update_script_index_for_both_players(Object *object) {
    Object *pair = &player_left;

    set_script_table_index_from_thresholds(object, pair);
    set_script_table_index_from_thresholds(object, pair + 1);
}

void set_script_table_index_from_thresholds(Object *object, Object *other) {
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
