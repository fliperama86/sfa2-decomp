/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126130(void) {
    s16 i, j;
    game_state.field_64 = 0xff;
    data_8019032e = 1;
    data_80190111 = 1;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    data_8018f598 = 1;
    for (i = 0; i < 32; i++)
        for (j = 0; j < 16; j++) {
            data_801a27e4_rows[0][i * 16 + j] = 0x7fff;
            data_801a27e4_rows[1][i * 16 + j] = 0x7fff;
            data_801a27e4_rows[2][i * 16 + j] = 0x7fff;
            data_801a27e4_rows[3][i * 16 + j] = 0x7fff;
            data_801a27e4_rows[4][i * 16 + j] = 0x7fff;
        }
    func_801260ac(0, 1, 0x15);
    func_801260ac(0, 1, 0x17);
    func_80137b10();
}
