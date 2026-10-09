/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8013fab4(Object *object) {
    if (ref_other.p->field_7e != 0) {
        ref_other.p->field_c6 -= 0x30;
        if (*(s16 *)&ref_other.p->field_c6 < 0) ref_other.p->field_c6 = 0;
    }
    ref_other.p->field_04 = 1;
    ref_other.p->field_05 = 3;
    ref_other.p->field_06 = 0;
    ref_other.p->field_07 = 0;
    ref_other.p->field_73 = 0xff;
    object->field_73 = 1;
    ref_other.p->field_15b = 1;
    ref_other.p->field_46 = 4;
    object->field_28c = 0;
    object->field_28d = 0;
    object->field_297 = 0;
    ref_other.p->other = object;
    func_80138ac8((GameState *)game_state.config, object);
    object->field_159 = 0;
    ref_other.p->field_159 = 0;
    object->field_225 = 0;
    ref_other.p->field_225 = 0;
    ref_other.p->field_180 = 0;
    ref_other.p->field_2a1 = 0;
    ref_other.p->field_2a2 = 0;
    return 1;
}
