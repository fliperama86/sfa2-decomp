/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ca048_slot05_06(Object *obj);

void func_801c9c5c_slot05_06(Object *o) {
    ((Slot04aObj *)o)->field_1c3 = 0x32;
    o->field_07++;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    func_801204f4(o, o->side, 7);
    o->field_12c = 0;
    o->field_12d = 0;
    o->field_12e = 0;
    o->field_12f = 0;
    o->field_54 = -0x6000;
    func_801ca048_slot05_06(o);
    func_801307e0(o, (o->field_12a >> 1) + 0x34);
}
