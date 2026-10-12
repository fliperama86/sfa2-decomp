/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014780c(Object *object) {
    int zero = 0;
    game_state.other_object = (Object *)func_8011f1e0();
    if (game_state.other_object != 0) {
        game_state.other_object->active_count = game_state.other_object->active_count + 1;
        if (object->kind == 2) {
            game_state.other_object->field_02 = 0xa;
        } else {
            game_state.other_object->field_02 = 0x13;
        }
        game_state.other_object->sound_side = object->sound_side;
        game_state.other_object->mode_id = 0x20;
        game_state.other_object->table_index = 0;
        game_state.other_object->facing_side = game_state.other_object->field_0e = game_state.other_object->field_0f = zero;
        game_state.other_object->ot_depth = 0x1a;
        game_state.other_object->field_90 = object->field_90;
        game_state.other_object->data_ptr_a = object->data_ptr_a;
        game_state.other_object->data_ptr_b = object->data_ptr_b;
        game_state.other_object->spawn_x = object->spawn_x;
        game_state.other_object->spawn_y = object->spawn_y;
        game_state.other_object->parent = object;
    }
}
