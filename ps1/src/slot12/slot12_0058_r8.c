/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002d57c_slot12;
extern Object *data_8002d574_slot12;
void func_80010978_slot12(void);
void func_800109dc_slot12(void);

void func_800107d4_slot12(void) {
    if (game_state.field_27 & 0x80) {
        data_8002d574_slot12->field_00 = 0;
        func_80010978_slot12();
    } else if (game_state.field_08 == 0) {
        func_800109dc_slot12();
    }
}
