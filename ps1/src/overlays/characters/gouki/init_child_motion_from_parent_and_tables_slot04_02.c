/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c7514_slot04_02[];
extern u16 data_801c7748_slot04_02[];
extern u16 data_801c7754_slot04_02[];
extern s32 data_801c7760_slot04_02[];
extern s32 data_801c779c_slot04_02[];

int dispatch_table_pick_by_table_index_slot04_02(Object *object, Object *parent);
void set_sequence_a_by_sound_side_slot04_02(Object *object, u8 a_arg);

/* The local t holds the 0x7754 table value for the y offset; d holds the later x offsets. Written with one local for both, this function differs from the original in 16 instruction slots. */
void init_child_motion_from_parent_and_tables_slot04_02(Object *object) {
    Object *parent;
    u16 d;
    int t;

    ((Slot04aObj *)object)->data_pointer = data_801c7514_slot04_02;
    object->state_0++;
    ((Slot04aObj *)object)->field_b0 = object->image_row;
    parent = object->parent;
    object->ot_depth = 0;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    if (object->table_index != 0) {
        dispatch_table_pick_by_table_index_slot04_02(object, parent);
    }
    t = data_801c7754_slot04_02[object->table_index >> 1];
    object->vel_y = data_801c779c_slot04_02[object->vel_table_index];
    object->pos_y -= t;
    if (object->facing_side != 0) {
        d = data_801c7748_slot04_02[object->table_index >> 1];
        object->vel_x = data_801c7760_slot04_02[object->vel_table_index];
    } else {
        d = data_801c7748_slot04_02[object->table_index >> 1];
        d = -d;
        object->vel_x = -data_801c7760_slot04_02[object->vel_table_index];
    }
    object->pos_x += d;
    if (parent->side == 0) {
        ((Slot04aObj *)object)->scratchpad_value = (int)data_1f8000a8;
    } else {
        ((Slot04aObj *)object)->scratchpad_value = (int)data_1f800158;
    }
    set_sequence_a_by_sound_side_slot04_02(object, object->vel_table_index);
    if (object->table_index == 2 && ((Slot04aObj *)parent)->parent_state == -1) {
        if (parent->facing_side != 0) {
            object->vel_x -= 0x10000;
        } else {
            object->vel_x += 0x10000;
        }
    }
}
