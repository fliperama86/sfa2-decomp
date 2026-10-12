/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The cast of t to s16 in the call of set_sequence_from_table_5: without it this function differs from the original in 9 instruction slots. The local x is decremented in a statement of its own: written x = parent->countdown - 1, 2 instruction slots differ. The local t is reused for the sum with image_row: with a local of its own, 21 instruction slots differ. The sum is formed as t += v, then stored: written as t + v in the store, 12 instruction slots differ. */
void follow_parent_frame_param_sequence_slot04_0a(Object *obj, Object *parent) {
    int t;
    s16 x;

    obj->field_01 = 0;
    if (obj->table_index != parent->kind) {
        obj->state_0++;
    } else {
        t = parent->frame->frame_param & 0x7f;
        if (t == 0) {
            obj->variant_index = 0;
        } else {
            obj->pos_x = parent->pos_x;
            obj->pos_y = parent->pos_y;
            obj->facing_side = parent->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != t) {
                obj->countdown = 3;
                obj->image_row = parent->image_row;
                obj->variant_index = t;
                if (parent->side == 0) {
                    set_sequence_from_table_5(obj, (s16)t, data_1f8000b4);
                } else {
                    set_sequence_from_table_5(obj, (s16)t, data_1f800164);
                }
            } else if (((Slot04aObj *)obj)->mode == 0) {
                x = parent->countdown;
                x--;
                parent->countdown = x;
                if (x == 0) {
                    int v;

                    t = parent->image_row;
                    v = obj->vel_x ^ 1;
                    obj->countdown = 3;
                    obj->vel_x = v;
                    t += v;
                    obj->image_row = t;
                }
                advance_sequence_checked(obj);
            } else {
                obj->image_row = parent->image_row;
                advance_sequence_checked(obj);
            }
        }
    }
}
