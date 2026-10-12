/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c5e30_slot04_0f[];
extern s32 data_801c5e48_slot04_0f[];
extern u8 data_801c5e54_slot04_0f[];

void call_handler_by_state_1_slot04_0f(Object *obj);

void init_child_object_from_parent_slot04_0f(Object *obj) {
    Object *parent;
    int dx;
    int i;

    parent = obj->parent;
    obj->ot_depth = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c5e30_slot04_0f;
    obj->active_state = 0;
    obj->vel_y = 0;
    i = data_801c5e48_slot04_0f[obj->vel_table_index >> 1];
    dx = 0x53;
    if (obj->facing_side == 0) {
        i = -i;
        dx = -0x53;
    }
    obj->vel_x = i;
    obj->pos_x = obj->pos_x + dx;
    obj->pos_y = obj->pos_y - 0x39;
    obj->variant_index_2 = data_801c5e54_slot04_0f[obj->vel_table_index >> 1];
    if (parent->side == 0) {
        ((Slot04bObj *)obj)->data_pointer = *(int *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->data_pointer = *(int *)0x1f800158;
    }
    func_80138070(obj, (obj->vel_table_index >> 1) + 8);
    call_handler_by_state_1_slot04_0f(obj);
}
