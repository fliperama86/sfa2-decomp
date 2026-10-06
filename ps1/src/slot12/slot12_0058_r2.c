/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_8002d56c_slot12;
extern Object *data_8002d568_slot12;

void func_80010100_slot12(void) {
    HudState *h = data_8018f5a0;
    int one = 1;
    data_80190568 = one;
    h->field_4e++;
    game_state.field_09 = one;
    game_state.field_2c = 0xff;
    game_state.field_65 = one;
    func_8011a744();
    func_8011eb4c();
    data_8002d56c_slot12 = game_state.field_154;
    data_8002d568_slot12 = &player_left + (game_state.field_154->side ^ 1);
}

void func_800101b4_slot12(void) {
    data_8018f5a0->field_4e++;
}

void func_800101d4_slot12(void) {
    HudState *h = data_8018f5a0;
    h->field_4c++;
    h->field_4e = 0;
    game_state.field_c8 = 0;
    h->field_50 = 0;
    game_state.field_ca = 0;
    game_state.field_65 = 0;
    func_8011eae4();
}
