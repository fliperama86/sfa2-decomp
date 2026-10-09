/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8012f408(Object *o) {
    int a;
    u8 side;
    int k;
    if (game_state.config->field_04 == 0) return 0;
    side = o->field_158;
    k = o->kind;
    o->field_04 = 1;
    o->field_07 = 3;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_0b = side;
    if (k == 2) o->field_0b = side ^ 1;
    ref_other.p = o->other;
    if ((s16)o->field_5c == (s16)ref_other.p->field_5c) {
        a = 0x29;
    } else if ((s16)ref_other.p->field_5c < (s16)o->field_5c) {
        a = 0x23;
        if (o->kind == 0xd) a = 0x27;
        o->field_0b = o->field_158;
    } else {
        a = 0x28;
    }
    func_80130678(o, a);
    return 1;
}
