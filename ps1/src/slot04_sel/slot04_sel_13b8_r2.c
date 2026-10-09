/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801b9d24_slot04_sel;

void func_801b16b4_slot04_sel(void) {
    Object *r = &player_right;
    game_state.field_09 = 0xff;
    if (player_left.field_cd == 0) {
        game_state.mode |= 1;
    } else {
        game_state.mode &= 0xfe;
    }
    if (r->field_cd == 0) {
        game_state.mode |= 2;
    } else {
        game_state.mode &= 0xfd;
    }
    func_801257f4((Select *)&game_state);
    data_801b9d24_slot04_sel = 0x40;
    data_8018f5a0->field_50++;
}
