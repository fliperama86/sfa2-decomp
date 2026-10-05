/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801387c0(Object *object) {
    if (player_left.field_cd != 0) {
        if (player_left.kind == 0x14) {
            player_left.field_cf = 0x1f;
        } else {
            player_left.field_cf = ((u8 *)object)[0x6e];
        }
    }
    if (player_right.field_cd != 0) {
        if (player_right.kind == 0x14) {
            player_right.field_cf = 0x1f;
        } else {
            player_right.field_cf = ((u8 *)object)[0x6e];
        }
    }
}
