/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8013fab4(Object *object) {
    if (ref_other.p->field_7e != 0) {
        ref_other.p->meter -= 0x30;
        if (*(s16 *)&ref_other.p->meter < 0) ref_other.p->meter = 0;
    }
    ref_other.p->state_0 = 1;
    ref_other.p->state_1 = 3;
    ref_other.p->state_2 = 0;
    ref_other.p->state_3 = 0;
    ref_other.p->status_flag = 0xff;
    object->status_flag = 1;
    ref_other.p->field_15b = 1;
    ref_other.p->countdown = 4;
    object->field_28c = 0;
    object->damage_countdown = 0;
    object->reference_count = 0;
    ref_other.p->other = object;
    func_80138ac8((GameState *)game_state.config, object);
    object->active_flag_3 = 0;
    ref_other.p->active_flag_3 = 0;
    object->field_225 = 0;
    ref_other.p->field_225 = 0;
    ref_other.p->field_180 = 0;
    ref_other.p->cursor_timer_a = 0;
    ref_other.p->cursor_timer_b = 0;
    return 1;
}
