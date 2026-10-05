/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801285e0(void) {
    s16 v;
    int t;
    if (data_80197f10 >= 6) {
        t = game_state.field_48 * 2 - 1;
        v = game_state.field_42;
        if (v != t) {
            func_80144f10(0, 0);
            func_80144f10(1, (u8)(v + 2));
        } else {
            func_80144f10(0, 1);
        }
        func_80144f10(9, 0);
        func_80144f10(10, 0);
    }
    func_8012867c();
}
