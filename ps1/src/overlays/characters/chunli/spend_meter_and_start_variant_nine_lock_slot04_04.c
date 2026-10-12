/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_action_variant_9_lock_other_slot04_04(Object *obj);

int spend_meter_and_start_variant_nine_lock_slot04_04(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_action_variant_9_lock_other_slot04_04(obj);
    }
    return r;
}

void start_action_variant_9_lock_other_slot04_04(Object *obj) {
    ref_other.p = obj->other;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->active_flag_3 = 1;
    obj->field_157 = 0;
    obj->direction_index = 4;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    ref_other.p->field_6b = 0x14;
    ref_other.p->other_lock = 0x18;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x1e);
}
