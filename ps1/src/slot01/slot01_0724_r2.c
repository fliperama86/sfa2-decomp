/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80010838_slot01(void) {
    if (*(s16 *)&data_801ae066 != 0) {
        data_8018f5a0->field_4e++;
    }
}

void func_80010870_slot01(void) {
    if (game_state.field_06 == 0) {
        HudState *h = data_8018f5a0;
        h->field_4c++;
        h->field_4e = 0;
        game_state.field_c8 = 0;
        h->field_50 = 0;
        game_state.field_ca = 0;
        h->field_52 = 0;
        game_state.field_cc = 0;
        h->field_52 = 0;
        game_state.field_65 = 0;
    }
}
