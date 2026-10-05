/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Object *func_8011f1e0(void);

void func_8014780c(Object *object) {
    int zero = 0;
    game_state.field_358 = func_8011f1e0();
    if (game_state.field_358 != 0) {
        game_state.field_358->field_00 = game_state.field_358->field_00 + 1;
        if (object->kind == 2) {
            game_state.field_358->field_02 = 0xa;
        } else {
            game_state.field_358->field_02 = 0x13;
        }
        game_state.field_358->field_66 = object->field_66;
        game_state.field_358->field_08 = 0x20;
        game_state.field_358->field_03 = 0;
        game_state.field_358->field_0b = game_state.field_358->field_0e = game_state.field_358->field_0f = zero;
        game_state.field_358->field_09 = 0x1a;
        game_state.field_358->field_90 = object->field_90;
        game_state.field_358->field_98 = object->field_98;
        game_state.field_358->field_9c = object->field_9c;
        game_state.field_358->field_7a = object->field_7a;
        game_state.field_358->field_7c = object->field_7c;
        game_state.field_358->field_3c = object;
    }
}
