/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801beeb0_slot04_01[];
extern s8 data_801beec0_slot04_01[];

void func_801b0220_slot04_01(Object *obj) {
    int v;

    obj->field_46 = 0x3c;
    obj->field_06 = obj->field_06 + 1;
    if (game_state.config->field_76 == 0) {
        game_state.config->field_76 = 0x1e;
    }
    v = (s8)func_80125734(obj, data_801beeb0_slot04_01[func_80151184() & 0xf]);
    func_80130678(obj, data_801beec0_slot04_01[v]);
}
