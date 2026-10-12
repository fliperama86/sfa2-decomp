/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801ebbb8_slot06_10[];
extern s16 data_801f67f0_slot06_10;
extern s16 data_801f67f4_slot06_10;

void store_ordered_pos_x_by_facing_side_slot06_10(Object *obj, Object *other);

void pick_sequence_by_x_order_when_apart_slot06_10(Object *obj, Object *other) {
    s16 d;

    if ((s16)obj->step_flags & 0x8000) {
        d = other->pos_x - obj->pos_x;
        if (d < 0) {
            d = -d;
        }
        if ((s16)(d - 0x40) >= 0) {
            store_ordered_pos_x_by_facing_side_slot06_10(obj, other);
            if (data_801f67f0_slot06_10 > data_801f67f4_slot06_10) {
                d = 0x10;
                obj->state_1 = 0;
            } else {
                d = 0xe;
                obj->state_1 = 2;
            }
            set_sequence_entry(obj, data_801ebbb8_slot06_10[d]);
            return;
        }
    }
    advance_sequence_checked(obj);
}

void set_state_1_1_if_x_order_descending_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->step_flags & 0x8000) {
        store_ordered_pos_x_by_facing_side_slot06_10(obj, other);
        if (data_801f67f0_slot06_10 > data_801f67f4_slot06_10) {
            set_sequence_entry(obj, data_801ebbb8_slot06_10[0xf]);
            obj->state_1 = 1;
            return;
        }
    }
    advance_sequence_checked(obj);
}
