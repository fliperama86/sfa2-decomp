/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4508_slot04_04[];
extern s32 data_801c4520_slot04_04[];
extern u8 data_801c452c_slot04_04[];

void call_state_1_table_handler_then_cull_check_slot04_04(Object *obj);

void init_child_object_from_parent_with_tables_slot04_04(Object *obj) {
    Object *parent;
    u8 i;
    int k;

    parent = obj->parent;
    i = obj->vel_table_index;
    i >>= 1;
    obj->ot_depth = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04aObj *)obj)->data_pointer = data_801c4508_slot04_04;
    obj->active_state = 0;
    obj->vel_y = 0;
    k = data_801c4520_slot04_04[i];
    if (obj->facing_side != 0) {
        obj->vel_x = k;
        obj->pos_x = obj->pos_x + 0x58;
    } else {
        k = -k;
        obj->vel_x = k;
        obj->pos_x = obj->pos_x - 0x58;
    }
    obj->pos_y = obj->pos_y - 0x38;
    obj->variant_index_2 = data_801c452c_slot04_04[obj->vel_table_index >> 1];
    if (parent->side == 0) {
        ((Slot04aObj *)obj)->scratchpad_value = *(int *)0x1f8000a8;
    } else {
        ((Slot04aObj *)obj)->scratchpad_value = *(int *)0x1f800158;
    }
    set_sequence_by_sound_side(obj, obj->vel_table_index >> 1);
    call_state_1_table_handler_then_cull_check_slot04_04(obj);
}
