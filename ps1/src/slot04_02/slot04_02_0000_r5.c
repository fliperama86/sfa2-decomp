/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 try_start_meter_full_state_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x90) {
        r = try_start_effect_by_state(obj);
        if (r) {
            set_object_states_0_to_3_slot04_02(obj, 1, 0, 8, 0);
            obj->field_15a = 0xb;
            obj->field_255 = 4;
            obj->direction_index = 4;
            obj->facing_side = obj->saved_facing;
        }
    }
    return r;
}
