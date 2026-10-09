/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_8002ceb0_slot01;
extern Object *data_80055f40_slot01;

void func_80010498_slot01(void) {
    int t = game_state.field_c8 - 1;
    game_state.field_c8 = t;
    if ((s16)t < 0 || func_801252f0() != 0) {
        data_8018f5a0->field_4e++;
        game_state.field_ab = 0xff;
        if (game_state.mode != 0) {
            game_state.field_09 = 0;
        }
    }
}

void func_80010524_slot01(void) {
    Object *s0;
    Object *p;

    s0 = game_state.field_78;
    data_8018f5a0->field_4e++;
    game_state.field_c8 = 0xb4;
    data_8002ceb0_slot01->field_7c = 0x1e9;
    p = (Object *)func_8011f1e0();
    if (p) {
        p->field_00 = 1;
        p->field_02 = 0x1b;
        p->field_03 = 0x80;
        p->field_7a = 0x70;
        p->field_3c = s0;
        p->field_7c = 0x1ea;
        data_80055f40_slot01 = p;
    }
}
