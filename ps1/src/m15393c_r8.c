/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801564f0(void *a, Object *o) {
    u8 x, y;
    o->name_char_0 = 0x41;
    o->name_char_1 = 0x60;
    o->name_char_2 = 0x60;
    o->name_char_3 = 0x20;
    o->field_b4 = 0x960;
    *(s16 *)&o->field_b2 = -0x1000;
    o->field_123 = 0;
    o->field_b0 = 0x3c;
    o->phase_index++;
    func_80156898(a, o);
    func_80156a20(a, o);
    x = o->score_rank;
    y = o->field_121;
    if ((u8)(x + y) >= 12) {
        func_80156818(a, o);
    } else {
        if (y < x) {
            x = y;
        }
        o->min_rank = x;
    }
}
