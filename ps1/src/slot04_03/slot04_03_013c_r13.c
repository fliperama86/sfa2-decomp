/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f98_slot04_03(Object *obj) {
    int a = 0x2b;
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    }
    set_sequence_by_side(obj, a);
}
