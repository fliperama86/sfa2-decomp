/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4290_slot04_02(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            game_state.field_63 = 0x18;
            func_801204f4(obj, obj->side, 0xe);
        }
        func_80130efc(obj);
    }
}
