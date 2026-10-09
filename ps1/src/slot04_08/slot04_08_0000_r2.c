/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3b48_slot04_08[];

u8 func_80125734(Object *object, int a);

void func_801b01d8_slot04_08(Object *obj) {
    u8 k;

    obj->field_46 = 0x3c;
    obj->field_06 = obj->field_06 + 1;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    k = func_80125734(obj, data_801c3b48_slot04_08[func_80151184() & 0xf]);
    if (k == 3 && game_state.field_0a != 0) {
        k = 4;
    }
    obj->field_48 = k;
    func_80130678(obj, k + 0x23);
}
