/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002f158_slot27;

/* One local holds two values in turn: the mode value that the first test
   compares, and then the byte that goes to field_40. With a second local for
   the byte, this function differs from the original in 11 instruction slots;
   with the first test written without a local, in 15. */
void func_80010840_slot27(void) {
    int value;
    data_8002f158_slot27 = 0;
    value = game_state.mode | game_state.field_07;
    if (data_8018f5a0->field_52 == value) {
        data_8018f5a0->field_4e++;
        game_state.field_09 = 1;
        func_801257f4((Select *)&game_state);
        if (game_state.field_b0 != 0) {
            value = game_state.field_b1;
            if ((value & 0x80) == 0) {
                if (game_state.field_b2 != 0) {
                    if (game_state.field_b2 == 10) {
                        value = 0x13;
                    }
                    if (game_state.field_b2 == 11) {
                        value = 0x12;
                    }
                }
                game_state.field_40 = value;
            }
        }
        game_state.field_c6 = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 0;
        data_8018f5a0->field_54 = 0;
        data_8018f5a0->field_4e++;
        if ((game_state.mode | game_state.field_07) == 3) {
            data_8018f5a0->field_4e++;
            game_state.field_c6 = 0x40;
        }
    }
}
