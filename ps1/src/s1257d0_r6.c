/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126244(void) {
    if (game_state.field_64 & 0x80) game_state.field_64 = 0;
    data_8019032e = 0;
    data_80190111 = 0;
    player_left.field_01 = 1;
    player_right.field_01 = 1;
    data_8018f598 = 0;
    func_801260ac(0, 0x20, 0);
    func_801260ac(1, 0x20, 0);
    func_801260ac(2, 0x20, 0);
    func_801260ac(3, 0x20, 0);
    func_801260ac(4, 0x20, 0);
    func_80137b10();
}
