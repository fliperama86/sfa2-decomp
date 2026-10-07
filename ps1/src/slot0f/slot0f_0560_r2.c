/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800e84f0_slot0f[];

void func_800e06b8_slot0f(GameState *g, int x) {
    g->field_1d++;
    g->field_a8 = x;
    func_80128978();
    func_8012510c();
    g->field_65 = player_left.field_165 | player_right.field_165;
    func_80138290();
    func_8013902c();
    func_80138164();
    func_801510bc();
    if (data_80190568 != 0) {
        func_80135ef0(g);
    }
    if (g->field_63 != 0) {
        g->field_63--;
    }
    func_80138ec4();
    func_80132e84();
    func_8011a784();
}

void func_800e0784_slot0f(int i) {
    func_801519b4(data_800e84f0_slot0f[i]);
}
