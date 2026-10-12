/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void reset_substate_and_set_direction_index_slot04_0e(Object *obj);
int try_start_action_3_with_variant_slot04_0e(Object *obj);

int try_start_action_3_with_variant_slot04_0e(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->field_15a = 3;
    obj->active_flag_3 = 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->variant_index = 0xff;
    if (!(obj->input_bits_2 & 0x2000)) {
        obj->variant_index = 1;
    }
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}
