/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4f7c_slot04_06(Object *obj) {
    if (((Slot04aObj *)obj)->field_47 != 0) {
        ((Slot04aObj *)obj)->field_47--;
        if (((Slot04aObj *)obj)->field_47 == 0) {
            game_state_second.field_4b = game_state.field_4b | (1 << obj->side);
        }
    }
    func_80130efc(obj);
}
