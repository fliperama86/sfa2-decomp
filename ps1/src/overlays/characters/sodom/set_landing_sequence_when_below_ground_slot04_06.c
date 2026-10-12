/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_y_minus_ground_slot04_06(Object *obj);

void set_landing_sequence_when_below_ground_slot04_06(Object *obj) {
    int a;
    int t;

    if ((move_by_velocity_return_y_minus_ground_slot04_06(obj) << 16) > 0) {
        t = obj->state_1;
        a = obj->table_index;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        t++;
        a = (a & 1) | 0x18;
        obj->state_1 = t;
        if (obj->sound_side == 0) {
            set_sequence_from_table_5(obj, a, data_1f8000b4);
        } else {
            set_sequence_from_table_5(obj, a, data_1f800164);
        }
    }
    advance_sequence_checked(obj);
}
