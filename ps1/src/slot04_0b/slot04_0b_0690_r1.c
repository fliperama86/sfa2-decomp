/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c24b8_slot04_0b[];
extern s8 data_801c24c8_slot04_0b[];
u8 func_80125734(Object *object, int a);
void func_80130678(Object *object, int arg);

void func_801b0690_slot04_0b(Object *obj) {
    int r;

    obj->field_06 = obj->field_06 + 1;
    obj->field_46 = 0x3c;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    r = func_80125734(obj, data_801c24b8_slot04_0b[func_80151184() & 0xf]);
    func_80130678(obj, data_801c24c8_slot04_0b[(s8)r]);
}
