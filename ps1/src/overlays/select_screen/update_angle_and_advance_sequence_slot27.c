/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void copy_side_palette_from_table_and_upload_slot27(Object *obj);

void update_angle_and_advance_sequence_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;

    int sum = obj->angle + obj->field_4c;
    int x = (s16)sum;

    x -= 0xe8;
    obj->angle = sum;
    if ((u32)x >= 0x240) {
        o->state_0 = o->state_0 + 1;
    }
    advance_sequence_checked(o);
    clear_or_set_flag_by_pos_x_limit(o);
}
