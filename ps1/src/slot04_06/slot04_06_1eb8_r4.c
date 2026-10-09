/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2274_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->field_07++;
    obj->field_1c3 = 0x32;
    func_80145d20(o);
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_801204f4(o, o->side, 7);
    func_801307e0(o, 0x22);
}
