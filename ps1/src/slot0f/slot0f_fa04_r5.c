/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800e8404_slot0f[])(GameState *, Menu *);
void func_800e009c_slot0f(Object *obj, int a);
void func_800e27a4_slot0f(void);
void func_800e1640_slot0f(void);

void func_800dfee0_slot0f(GameState *state, Menu *menu) {
    data_800e8404_slot0f[(s8)menu->mode_index](state, menu);
}

void func_800dff20_slot0f(GameState *state, Menu *menu) {
    state->side_bits = menu->active_flag;
    state->versus_flag = 0;
    state->mode3_active_flag = 0;
    func_800e009c_slot0f((Object *)state, 0);
}

void func_800dff50_slot0f(GameState *state, Menu *menu) {
    state->side_bits = 3;
    state->mode3_active_flag = 1;
    state->versus_flag = 0;
    func_800e009c_slot0f((Object *)state, 2);
}

void func_800dff84_slot0f(GameState *state, Menu *menu) {
    state->side_bits = menu->active_flag;
    state->mode3_active_flag = 0;
    state->versus_flag = 1;
    func_800e009c_slot0f((Object *)state, 2);
}

void func_800dffb8_slot0f(GameState *state, Menu *menu) {
    func_800e27a4_slot0f();
}

void func_800dffd8_slot0f(GameState *state, Menu *menu) {
    data_8018f5a0->phase_index--;
    func_800e1640_slot0f();
}
