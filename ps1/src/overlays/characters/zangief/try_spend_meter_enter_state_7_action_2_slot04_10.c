/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../externs.h"

void set_sequence_by_side_and_mode(Object *object, int arg);
int try_spend_meter_for_action(Object *object);
/* functions of other units of this module */
u8 try_spend_meter_enter_state_7_action_2_slot04_10(Object *obj);

u8 try_spend_meter_enter_state_7_action_2_slot04_10(Object *obj) {
    if (try_spend_meter_for_action(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = 1;
        obj->active_flag_4 = 1;
        obj->direction_index = 4;
        obj->other_lock = 0x11;
        obj->field_157 = 0;
        obj->field_6b = 0;
        obj->facing_side = obj->saved_facing;
        ref_other.p = obj->other;
        ref_other.p->field_6b = 0xd;
        set_sequence_by_side_and_mode(obj, 0x20);
        return 1;
    }
    return 0;
}
