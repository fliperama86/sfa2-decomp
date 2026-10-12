/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_801410c8(Object *object) {
    if (game_state.config->field_4d == 0) {
        if (object->field_cd != 0) return drain_gauge(object) & 0xff;
        ref_other.p = object->other;
        if (ref_other.p->pressed_bits & 0xf000) {
            if ((s16)(*(s16 *)&object->gauge -= 3) < 0) goto reset;
        }
        if (*(u8 *)&ref_other.p->pressed_bits & 0xfc) {
            if ((s16)--*(s16 *)&object->gauge < 0) goto reset;
        }
        if ((s16)--*(s16 *)&object->gauge >= 0) return 0;
    }
reset:
    object->gauge = 0;
    return 1;
}
