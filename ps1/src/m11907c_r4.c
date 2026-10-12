/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011a784(void) {
    int n = data_801a27d0;
    data_801a4fe8 = (u8 *)data_801987cc[n];
    data_801a697c = (u8 *)data_8018db18[n];
    data_801ad3dc = (u8 *)&data_801987cc[n][0x280];
    data_801ae118 = (u8 *)data_8019056c[n];
    if (game_state.setup_done_flag == 0) {
        func_80134234();
        func_8011a880();
        func_8011acbc();
        func_8011b594();
        func_8011b708();
        func_80136ccc();
        game_state.setup_done_flag = 1;
        data_801a695c = 0;
    }
    data_801aa540 = 0;
    func_8011ff24();
}
