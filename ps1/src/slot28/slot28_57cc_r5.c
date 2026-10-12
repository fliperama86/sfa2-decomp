/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void blink_object_then_advance_sequence_stage_slot28(Object *obj) {
    if (--obj->vel_x == 0) {
        obj->vel_x = 0x20;
        obj->state_1 = 0;
        if (((u8 *)&obj->countdown)[1] == 2) {
            obj->field_01 = 0;
            obj->state_0++;
        } else {
            ((u8 *)&obj->countdown)[1]++;
            if (((u8 *)&obj->countdown)[1] == 2) {
                *(u16 *)&obj->pos_y -= 8;
            }
            set_sequence_from_table_5(obj, ((u8 *)&obj->countdown)[1] + 7, (SequenceStep **)obj->box_tables);
        }
    } else {
        obj->variant_index = obj->variant_index ^ 1;
        if (obj->variant_index != 0) {
            obj->field_01 = 1;
        } else {
            obj->field_01 = 0;
        }
    }
}

void drift_right_and_blink_field_slot28(Object *obj) {
    *(int *)&obj->pos_x_frac += 0x4000;
    if (obj->pos_x > 0x180) {
        obj->state_0++;
    }
    obj->variant_index = obj->variant_index ^ 1;
    if (obj->variant_index != 0) {
        obj->field_01 = 0;
    } else {
        obj->field_01 = 1;
    }
    advance_sequence_checked(obj);
}
