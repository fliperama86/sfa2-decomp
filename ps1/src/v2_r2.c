/* Reconstruction. Names/roles inferred, not original symbols.
 * The original takes the address of the left player's mode_flag_2 once and derives
 * both player addresses from it. Selecting the player through its fields did not
 * match, so the selection is written from that address. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f038(void) {
    GameState *g = &game_state;
    Object *p;
    int kind;
    u8 *flag;
    game_state.loop_mode = 2;
    flag = &player_left.mode_flag_2;
    p = (Object *)(flag - 0xcd);     /* the left player, whose mode_flag_2 this is */
    if (*flag) {
        p = (Object *)(flag - 0xcd) + 1;     /* the right player follows the left */
    }
    kind = p->kind;
    if (game_state.stage_index >= 0x2a) {
        kind += 0x2a;
    } else if (game_state.stage_index >= 0x15) {
        kind += 0x15;
    }
    func_8014f408(8, kind);
    g->loop_mode = 0;
}
