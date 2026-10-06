/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern int data_8002d57c_slot12;
extern Object *data_8002d574_slot12;
void func_80010978_slot12(void);
void func_800109dc_slot12(void);

void func_80010744_slot12(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    if (game_state.field_f0 == 0) {
        data_8002d57c_slot12 = 1;
        if (game_state.field_06 == 0) {
            if (game_state.field_5f != 0) {
                data_8018f5a0->field_4e++;
                game_state.field_09 = 0;
                game_state.field_2c = 0;
            } else {
                func_800109dc_slot12();
            }
        }
    }
}
