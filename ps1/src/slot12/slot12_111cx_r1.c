/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80011190_slot12(void);

void func_8001111c_slot12(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    if (func_80125268() == 0) {
        u16 *t = &game_state.field_c6;
        int v = *t - 1;

        *t = v;
        if ((s16)v >= 0) {
            return;
        }
    }
    data_8018f5a0->field_4c++;
    func_8014f4d4(6, 2);
    func_80011190_slot12();
}
