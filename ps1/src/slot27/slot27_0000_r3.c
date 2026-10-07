/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_80029324_slot27;
void func_80010e44_slot27(void);
void func_80010ed0_slot27(void);
void func_80010fa8_slot27(void);
void func_80011118_slot27(void);
void func_800111c8_slot27(void);

void func_8001049c_slot27(void) {
    HudState *h = data_8018f5a0;
    data_80029324_slot27 = 0xff;
    h->field_50++;
}

void func_800104c4_slot27(void) {
    data_8018f5a0->field_50++;
    func_80010e44_slot27();
    game_state.field_06 = 0;
    game_state.field_09 = 0;
    game_state.field_c6 = 0x2b;
}

void func_80010514_slot27(void) {
    int t = game_state.field_c6 - 1;
    game_state.field_c6 = t;
    if ((s16)t == 0) {
        data_8018f5a0->field_50++;
        func_80010ed0_slot27();
        func_80011118_slot27();
        game_state.field_c8 = 8;
    }
}

void func_80010584_slot27(void) {
    int t = game_state.field_c8 - 1;
    game_state.field_c8 = t;
    if ((s16)t == 0) {
        HudState *h = data_8018f5a0;
        h->field_50 = 0;
        h->field_52 = 0;
        h->field_4e += 2;
        func_80010fa8_slot27();
        func_800111c8_slot27();
    }
}
