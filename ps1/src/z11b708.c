/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011b708(void) {
    u8 *z = (u8 *)data_8019045c;
    u8 *i;
    *z = 0;
    i = (u8 *)data_8019045c;
    do {
        if (data_801ac888[*i].field_00 != 0
            && data_801ac888[*i].field_01 != 0
            && data_801ac888[*i].field_81 == 4
            && data_80190568 != 0) {
            if (game_state.field_40 == 0) func_801e9080(&data_801ac888[*i]);
            if (game_state.field_40 == 1) func_801e9f90(&data_801ac888[*i]);
            if (game_state.field_40 == 2) func_801e99c8(&data_801ac888[*i]);
            if (game_state.field_40 == 3) func_801e9d04(&data_801ac888[*i]);
            if (game_state.field_40 == 4) func_801e99b4(&data_801ac888[*i]);
            if (game_state.field_40 == 5) func_801e8dc8(&data_801ac888[*i]);
            if (game_state.field_40 == 6) func_801e9798(&data_801ac888[*i]);
            if (game_state.field_40 == 7) func_801e9970(&data_801ac888[*i]);
            if (game_state.field_40 == 8) func_801e9b54(&data_801ac888[*i]);
            if (game_state.field_40 == 9) func_801e9840(&data_801ac888[*i]);
            if (game_state.field_40 == 10) func_801e8df0(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 11) func_801e96fc(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 12) func_801ea640(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 13) func_801e9f20(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 14) func_801e9d14(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 15) func_801e9bb0(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 16) func_801e9ef4(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 17) func_801e8fec(&data_801ac888[*(u8 *)data_8019045c]);
            if (game_state.field_40 == 18) func_801e9114(&data_801ac888[*(u8 *)data_8019045c]);
        }
        (*i)++;
    } while (*i < 16);
}
