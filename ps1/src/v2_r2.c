/* Reconstruction. Names/roles inferred, not original symbols.
 * The original takes the address of the left player's field_cd once and derives
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
    game_state.field_225 = 2;
    flag = &player_left.field_cd;
    p = (Object *)(flag - 0xcd);     /* the left player, whose field_cd this is */
    if (*flag) {
        p = (Object *)(flag - 0xcd) + 1;     /* the right player follows the left */
    }
    kind = p->kind;
    if (game_state.field_8b >= 0x2a) {
        kind += 0x2a;
    } else if (game_state.field_8b >= 0x15) {
        kind += 0x15;
    }
    func_8014f408(8, kind);
    g->field_225 = 0;
}
