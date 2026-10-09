/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
extern u8 data_801ac6a8;

void func_80122424(GameState *state) {
    if (data_80197fc4 != 0x701 && data_80198358 != 0x701) {
        if (((Attacker *)state)->field_84 != 0) {
            if (data_8018f5a0->field_52 == 0) {
                data_8018f5a0->field_52++;
                func_80125dc0(0, 0x20, 0xd, 0);
                func_80125dc0(1, 0x20, 0xd, 0);
                func_80125dc0(2, 0x20, 0xd, 0);
                func_80125dc0(3, 0x20, 0xd, 0);
                func_80137b10();
                data_801ac6a8 = 0;
                ((Attacker *)state)->field_84 = 0x80;
            } else if (data_8018f5a0->field_52 == 1) {
                data_8018f5a0->field_52++;
                func_8014f038();
            } else {
                func_801e0020((Attacker *)state);
            }
        } else {
            data_8018f5a0->field_50 = 3;
            ((Attacker *)state)->field_4f = 0;
            ((Attacker *)state)->field_09 = 0;
            ((Attacker *)state)->field_2c = 0;
            data_801ac6a8 = 1;
        }
    }
}
