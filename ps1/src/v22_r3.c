/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

/* Exact: the earlier candidate matches once the externs it needs are declared. */
void func_80128978(void) {
    game_state.config = &game_state_second;
    if (game_state.config->field_a8 == 0) {
        player_left.field_132 = player_left.field_130;
        player_right.field_132 = player_right.field_130;
        player_left.field_152 = player_left.field_150;
        player_right.field_152 = player_right.field_150;
        player_left.field_130 = data_801a6966;
        player_right.field_130 = data_801a6972;
        player_left.field_150 = data_801a6966;
        player_right.field_150 = data_801a6972;
        func_80128c48();
        if (player_left.field_73 == 0xff) {
            func_80128e1c();
        } else if (player_right.field_73 == 0xff) {
            func_80128e7c();
        } else if ((player_left.field_165 & player_right.field_165) == 0) {
            if (player_left.field_165 != 0) {
                func_80128e54();
            } else if (player_right.field_165 != 0) {
                func_80128eb4();
            } else if ((func_80151184() & 1) != 0) {
                func_80128bb8();
            } else {
                func_80128b28();
            }
        } else if ((func_80151184() & 1) != 0) {
            func_80128bb8();
        } else {
            func_80128b28();
        }
    }
}
