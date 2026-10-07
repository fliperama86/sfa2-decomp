/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800e8404_slot0f[])(GameState *, Menu *);
void func_800e009c_slot0f(Object *obj, int a);
void func_800e27a4_slot0f(void);
void func_800e1640_slot0f(void);

void func_800dfee0_slot0f(GameState *state, Menu *menu) {
    data_800e8404_slot0f[(s8)menu->field_00](state, menu);
}

void func_800dff20_slot0f(GameState *state, Menu *menu) {
    state->field_17 = menu->field_01;
    state->field_30 = 0;
    state->field_2f = 0;
    func_800e009c_slot0f((Object *)state, 0);
}

void func_800dff50_slot0f(GameState *state, Menu *menu) {
    state->field_17 = 3;
    state->field_2f = 1;
    state->field_30 = 0;
    func_800e009c_slot0f((Object *)state, 2);
}

void func_800dff84_slot0f(GameState *state, Menu *menu) {
    state->field_17 = menu->field_01;
    state->field_2f = 0;
    state->field_30 = 1;
    func_800e009c_slot0f((Object *)state, 2);
}

void func_800dffb8_slot0f(GameState *state, Menu *menu) {
    func_800e27a4_slot0f();
}

void func_800dffd8_slot0f(GameState *state, Menu *menu) {
    data_8018f5a0->field_4a--;
    func_800e1640_slot0f();
}
