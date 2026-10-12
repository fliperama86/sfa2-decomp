/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e483c_slot0f(void);
void func_800e4988_slot0f(void);
void func_800e009c_slot0f(Object *obj, int arg);

void func_800dfc28_slot0f(GameState *state, Menu *menu) {
    data_8018f5a0->step_index++;
    func_800e483c_slot0f();
    state->step_counter = 0x708;
}
