/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_8002d568_slot12;
extern Object *data_8002d56c_slot12;

void func_80010efc_slot12(void) {
    u16 *p = &game_state.field_c6;
    int t = *p - 1;
    *p = t;
    if ((s16)t < 0) {
        Block172 *b;
        data_8018f5a0->field_4c = data_8018f5a0->field_4c + 1;
        *p = 0x2f;
        if (game_state.field_44 & 0x80) {
            func_801204f4(data_8002d56c_slot12, game_state.field_227 ^ 1, 5);
        } else {
            func_801204f4(data_8002d568_slot12, data_8002d568_slot12->side, 5);
        }
        b = func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x26;
            b->field_03 = 1;
        }
        b = func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x7e;
            b->field_03 = 0;
        }
        b = func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x7e;
            b->field_03 = 1;
        }
    }
}
