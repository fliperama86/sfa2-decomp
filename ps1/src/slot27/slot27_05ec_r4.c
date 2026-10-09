/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_80017c04_slot27[])(void);
extern void (*data_80017c10_slot27[])(void);
void func_800110b8_slot27(void);

void func_8001096c_slot27(void) {
    data_80017c04_slot27[data_8018f5a0->field_50]();
}

void func_800109b4_slot27(void) {
    HudState *h = data_8018f5a0;
    h->field_50++;
    h->field_52 = 0;
    h->field_54 = 0;
    func_800110b8_slot27();
    game_state.field_c8 = 0;
}

void func_800109f4_slot27(void) {
    if (*(u8 *)&game_state.field_c8 == 3) {
        HudState *h = data_8018f5a0;
        h->field_50++;
        h->field_52 = 0;
        h->field_54 = 0;
        game_state.field_c6 = 0x80;
    }
}

void func_80010a3c_slot27(void) {
    u16 *p = &game_state.field_c6;
    int t = *p - 1;
    *p = t;
    if ((s16)t == 0) {
        HudState *h = data_8018f5a0;
        h->field_4e++;
        h->field_50 = 0;
        *p = 1;
    }
}

void func_80010a8c_slot27(void) {
    data_80017c10_slot27[data_8018f5a0->field_50]();
}
