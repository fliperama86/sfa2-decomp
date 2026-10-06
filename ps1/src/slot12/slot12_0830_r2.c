/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_8002d568_slot12;

void func_80010978_slot12(void) {
    data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
    func_8014f4d4(6, 2);
    game_state.field_c8 = 0x3c;
    game_state.field_ab = 0xff;
    func_801204f4(data_8002d568_slot12, data_8002d568_slot12->side, 8);
}

void func_800109dc_slot12(void) {
    func_8014f4d4(6, 2);
    data_8018f5a0->field_4a = 4;
    game_state.field_c4 = 0;
    data_8018f5a0->field_4c = 0;
    game_state.field_c6 = 0;
    data_8018f5a0->field_4e = 0;
    game_state.field_c8 = 0;
    data_8018f5a0->field_50 = 0;
    game_state.field_ca = 0;
    data_8018f5a0->field_52 = 0;
    game_state.field_cc = 0;
    game_state.field_27 = 0;
    game_state.field_2c = 0;
    game_state.field_50 = 0;
    game_state.field_65 = 1;
    game_state.field_ab = 1;
    game_state.field_4f = 0;
    game_state.field_09 = 1;
    game_state.field_46 = 0;
    game_state.field_44 = 1;
}
