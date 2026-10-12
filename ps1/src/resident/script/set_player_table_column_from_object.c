/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_player_table_column_from_object(Object *object) {
    if (player_left.mode_flag_2 != 0) {
        if (player_left.kind == 0x14) {
            player_left.table_column = 0x1f;
        } else {
            player_left.table_column = ((u8 *)object)[0x6e];
        }
    }
    if (player_right.mode_flag_2 != 0) {
        if (player_right.kind == 0x14) {
            player_right.table_column = 0x1f;
        } else {
            player_right.table_column = ((u8 *)object)[0x6e];
        }
    }
}
