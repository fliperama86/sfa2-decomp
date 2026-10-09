/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f98_slot04_03(Object *obj) {
    int a = 0x2b;
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    }
    func_80130678(obj, a);
}
