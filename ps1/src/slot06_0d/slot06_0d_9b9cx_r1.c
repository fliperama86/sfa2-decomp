/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9b9c_slot06_0d(Object *o) {
    int v;
    int w;

    /* Two locals for the two bytes, the first ORed in place with the second: as one expression seven instruction slots differ. */
    v = game_state.field_65;
    w = game_state.field_74;
    v |= w;
    if (v == 0) {
        v = o->field_3a;
        if ((v & 0xff) != 0) {
            o->field_3a = v & 0xff00;
            v = func_80151184() & 0xff;
            v += 0x40;
            o->field_38 = v;
        }
        func_80131094(o);
    }
    func_80120028(o);
}
