/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0728_slot04_0b(Object *obj) {
    s16 t = obj->field_46;
    if (t != 0) {
        t = t - 1;
        obj->field_46 = t;
        if (t == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    func_80130efc(obj);
}
