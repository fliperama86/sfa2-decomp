/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80011280_slot27(void);
void func_80010d7c_slot27(void);

void func_80010ad4_slot27(void) {
    Rect rect;
    HudState *h = data_8018f5a0;
    h->field_50++;
    rect.x = 0x40;
    rect.y = 0x1e0;
    rect.w = 0x10;
    game_state.field_80 = 0;
    rect.h = 1;
    func_80157fc4(&rect, (u8 *)(0x800ed820 + ((game_state.field_33 & 3) << 5)));
}

void func_80010b4c_slot27(void) {
    u16 *p = &game_state.field_c6;
    int t = *p - 1;
    *p = t;
    if ((s16)t == 0) {
        Block172 *b;
        data_8018f5a0->field_50++;
        func_8014f4d4(6, 2);
        func_80011280_slot27();
        b = func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x10;
            b->field_03 = 5;
        }
        game_state.field_06 = 0xff;
        game_state.field_07 = 0;
        game_state.field_1a = 0;
        game_state.field_2c = 1;
        func_80010d7c_slot27();
    }
}
