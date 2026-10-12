/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void play_object_sound_by_kind(Object *o) {
    int a, c;
    if (o->sound_select_flag != 0) {
        c = 0x32b;
        a = o->side;
    } else if (((0x10d40 >> o->kind) & 1) != 0) {
        c = 0x315;
        a = o->side;
    } else {
        a = o->side;
        c = 0x314;
    }
    play_sound_at(o, a, c);
}
