/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

int start_health_comparison_sequence(Object *o) {
    int a;
    u8 side;
    int k;
    if (game_state.config->mode_flag_a == 0) return 0;
    side = o->saved_facing;
    k = o->kind;
    o->state_0 = 1;
    o->state_3 = 3;
    o->state_1 = 0;
    o->state_2 = 0;
    o->facing_side = side;
    if (k == 2) o->facing_side = side ^ 1;
    ref_other.p = o->other;
    if ((s16)o->health == (s16)ref_other.p->health) {
        a = 0x29;
    } else if ((s16)ref_other.p->health < (s16)o->health) {
        a = 0x23;
        if (o->kind == 0xd) a = 0x27;
        o->facing_side = o->saved_facing;
    } else {
        a = 0x28;
    }
    set_sequence_by_side(o, a);
    return 1;
}
