/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b18bc_slot04_sel(void) {
    Object *p;
    u8 k;
    p = &player_left;
    k = player_left.kind;
    if (game_state.field_1c != 0 || player_left.field_118 != k) {
        player_left.field_ef = 1;
    }
    if (p->field_cd == 0) {
        p->field_118 = k;
        p->field_11a = p->field_d8;
        p->field_119 = p->field_d4;
    }
    p = &player_right;
    k = player_right.kind;
    if (game_state.field_1c != 0 || player_right.field_118 != k) {
        player_right.field_ef = 1;
    }
    if (p->field_cd == 0) {
        p->field_118 = k;
        p->field_11a = p->field_d8;
        p->field_119 = p->field_d4;
    }
}
