/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0510_slot04_03[];
extern s8 data_801c0520_slot04_03[];

u8 func_80125734(Object *object, int a);

void func_801b01c0_slot04_03(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_46 = 0x3c;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    func_80130678(obj, data_801c0520_slot04_03[func_80125734(obj, data_801c0510_slot04_03[func_80151184() & 0xf])]);
}
