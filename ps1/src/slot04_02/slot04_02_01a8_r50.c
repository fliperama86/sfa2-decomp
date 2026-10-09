/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b605c_slot04_02(Object *obj) {
    u16 t = obj->field_3a;
    s16 c;
    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
        game_state.field_63 = 0x18;
        func_801204f4(obj, obj->side, 0xe);
    }
    c = obj->field_46;
    if (c != 0) {
        c = c - 1;
        obj->field_46 = c;
        if (c == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    func_80130efc(obj);
}
