/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_80017bf0_slot27[])(void);
extern u8 data_80029324_slot27;
void func_80010ed0_slot27(void);

void func_800105ec_slot27(void) {
    data_80017bf0_slot27[data_8018f5a0->field_50]();
}

void func_80010634_slot27(void) {
    HudState *h = data_8018f5a0;
    h->field_50++;
    h->field_52 = 0;
    h->field_54 = 0;
    game_state.field_b0 = 1;
    game_state.field_c6 = 0;
    game_state.field_c8 = 0;
    game_state.field_80 = 0;
    game_state.field_b1 = 0xff;
    game_state.field_b2 = 0;
}

void func_80010694_slot27(void) {
    HudState *h = data_8018f5a0;
    Block172 *b;
    data_80029324_slot27 = 0xff;
    h->field_50++;
    b = func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_02 = 0xa8;
        b->field_03 = 0;
    }
    b = func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_02 = 0x10;
        b->field_03 = 7;
    }
    func_80010ed0_slot27();
    game_state.field_c8 = 8;
}
