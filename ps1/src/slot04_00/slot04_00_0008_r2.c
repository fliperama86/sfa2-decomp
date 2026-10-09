/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801bfd3c_slot04_00[];
extern s8 data_801bfd4c_slot04_00[];

void func_801b0240_slot04_00(Object *obj) {
    int r;

    obj->field_06 = obj->field_06 + 1;
    obj->field_46 = 0x3c;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    r = func_80125734(obj, data_801bfd3c_slot04_00[func_80151184() & 0xf]);
    func_80130678(obj, data_801bfd4c_slot04_00[(s8)r]);
}
