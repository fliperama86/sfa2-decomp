/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_8002d56c_slot12;
extern Object *data_8002d568_slot12;

void func_80011010_slot12(void) {
    u16 *t = &game_state.field_c6;
    int v = *t - 1;

    *t = v;
    if ((s16)v < 0) {
        data_8018f5a0->field_4c++;
        game_state.field_ab = 0xff;
        *t = 0x1f;
        if (game_state.field_44 & 0x80) {
            func_801204f4(data_8002d56c_slot12, game_state.field_227 ^ 1, 6);
        } else {
            func_801204f4(data_8002d568_slot12, data_8002d568_slot12->side, 6);
        }
    }
}

void func_800110c4_slot12(void) {
    u16 *t = &game_state.field_c6;
    int v = *t - 1;

    *t = v;
    if ((s16)v < 0) {
        data_8018f5a0->field_4c++;
        game_state.field_ab = 0;
        *t = 0x78;
    }
}
