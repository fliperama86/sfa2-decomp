/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
void func_800100a4(void);
void func_80010e60(void);

void func_80123804(GameState *state) {
    HudState *h;
    if (data_8016e68b == 0) {
        h = data_8018f5a0;
        if (h->field_52 == 0) {
            h->field_52++;
            func_8014ee5c(game_state.field_78->kind);
        } else {
            func_800100a4();
        }
    } else if (((Entity *)state)->field_27 == 0) {
        h = data_8018f5a0;
        h->field_4c++;
        ((Entity *)state)->field_65 = 0;
    } else {
        data_8018f5a0->field_4a = 3;
        data_8018f5a0->field_4c = 0;
        ((Entity *)state)->field_c6 = 0;
        data_8018f5a0->field_4e = 0;
        ((Entity *)state)->field_c8 = 0;
        data_8018f5a0->field_52 = 0;
        ((Entity *)state)->field_cc = 0;
        ((Entity *)state)->field_ab = 0;
        ((Entity *)state)->field_80 = 0;
        ((Entity *)state)->field_09 = 1;
        func_8011eb4c();
    }
}

void func_801238f4(GameState *state) {
    HudState *h;
    if (data_8016e68b == 0) {
        func_80010e60();
    } else {
        h = data_8018f5a0;
        h->field_4c = 0;
        ((Entity *)state)->field_c6 = 0;
        h->field_4e = 0;
        ((Entity *)state)->field_09 = 1;
        ((Entity *)state)->field_c8 = 0;
        ((Entity *)state)->field_80 = 0;
        ((Entity *)state)->field_2c = 0xff;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
    }
}
