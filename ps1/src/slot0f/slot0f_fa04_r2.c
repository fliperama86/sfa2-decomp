/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Menu data_800f8574_slot0f;
extern void (*data_800e83e8_slot0f[])(GameState *, Menu *);
extern void (*data_800e83f0_slot0f[])(GameState *, Menu *);
extern void (*data_800e83fc_slot0f[])(GameState *, Menu *);

void func_800dfae0_slot0f(void) {
    GameState *state = &game_state;
    Menu *menu = &data_800f8574_slot0f;
    data_800f8574_slot0f.active_flag = 0;
    menu->mode_index = 0;
    for (;;) {
        data_800e83e8_slot0f[data_8018f5a0->state_index](state, menu);
        get_random_byte();
        func_801192bc(1);
    }
}

void func_800dfb78_slot0f(GameState *state, Menu *menu) {
    data_8018f5a0->state_index++;
}

void func_800dfb98_slot0f(GameState *state, Menu *menu) {
    data_800e83f0_slot0f[data_8018f5a0->phase_index](state, menu);
}

void func_800dfbe0_slot0f(GameState *state, Menu *menu) {
    data_800e83fc_slot0f[data_8018f5a0->step_index](state, menu);
}
