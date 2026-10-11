/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b248c_slot04_0f(Object *o) {
    int a;
    int t;

    o->state_3 = o->state_3 + 1;
    o->field_17b = 1;
    func_80141f28(o, 4);
    func_80138ae8((GameState *)game_state.config, o);
    a = o->field_12a >> 1;
    /* Stand-in, not recovered source: the copy `t = a` before `a` takes the constant reproduces the original's choice of registers. Without it this function differs from the original in 3 instruction slots. */
    t = a;
    a = 0x3b;
    if (o->field_49 != 0) {
        a = 0x57;
    }
    func_801307e0(o, t + a);
}
