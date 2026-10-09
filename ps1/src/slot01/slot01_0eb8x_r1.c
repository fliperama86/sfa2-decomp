/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80010eb8_slot01(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    func_8011a744();
    data_8018f5a0->field_4e++;
    data_801aa65e[0] = 1;
    data_80190562[0] = 1;
    game_state.field_ab = 0;
    data_801aa5ce[0] = 0;
    data_801aa6ee[0] = 2;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    game_state.field_dc &= 0xffc0;
    func_8011eae4();
    func_80136e90();
    func_80136ed0();
    func_801260ac(1, 0x20, 0);
    func_801260ac(2, 0x20, 0);
    func_801260ac(3, 0x20, 0);
    func_80137220(0, 6);
    func_80137220(1, 0);
    func_80137220(2, 1);
    func_80137220(3, 2);
    func_80157d00(0);
}
