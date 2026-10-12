/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c5408_slot04_06[];

void count_down_then_launch_and_damage_other_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->vel_x = 0x18000;
        obj->vel_y = 0x8a000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
        obj->state_3 = obj->state_3 + 1;
        apply_damage_to_other_return_defeated(obj, *(s16 *)(data_801c5408_slot04_06 + (obj->direction_index & 0xfe)), 0);
        set_sequence_by_side_and_mode(obj, 0x23);
    } else {
        update_sequence(obj);
    }
}
