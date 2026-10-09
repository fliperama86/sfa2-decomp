/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80124220(void) {
    func_8012411c();
    data_80185fae[0] = 0;
    data_80185fae[3] = 0;
    data_8018f5a0->field_52++;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    if (player_left.kind == 0x13) {
        player_left.kind = 0x11;
    }
    if (player_right.kind == 0x13) {
        player_right.kind = 0x11;
    }
    game_state.field_b1 = 0xff;
    game_state.field_65 = 0xff;
    game_state.field_80 = 0;
    game_state.field_b0 = 0;
    game_state.field_b2 = 0;
    data_8016e800 = 0;
    game_state.field_6d = 0;
    data_80190568 = 1;
    func_8014f4d4(4, 0);
}
