/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80122d90(GameState *state) {
    if (((Entity *)state)->field_6c == 0) {
        if (((Entity *)state)->field_30 == 0) {
            ((HudBig *)data_8018f5a0)->field_4e++;
            ((Entity *)state)->field_c8 = 0;
            ((Entity *)state)->field_ca = 0;
            ((HudBig *)data_8018f5a0)->field_52 = 0;
            ((Entity *)state)->field_cc = 0;
            if (((Entity *)state)->field_a6 != 0 || ((Entity *)state)->field_78->field_cd != 0) {
                ((HudBig *)data_8018f5a0)->field_50 = 5;
            } else {
                ((HudBig *)data_8018f5a0)->field_50 = 0;
                ((Entity *)state)->field_ac = 0xff;
            }
        } else {
            int v = ((Entity *)state)->field_ca - 1;
            ((Entity *)state)->field_ca = v;
            if ((s16)v == 0) {
                func_80122e54(state);
            }
        }
    }
}
