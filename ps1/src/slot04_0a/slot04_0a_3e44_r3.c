/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0828_slot04_0a[];
void set_vel_x_from_table_by_facing_slot04_0a(Object *obj);

void init_object_at_parent_offset_slot04_0a(Object *obj) {
    int a;
    int y;

    obj->variant_index_2 = 0xff;
    ((Slot04aObj *)obj)->data_pointer = data_801c0828_slot04_0a;
    obj->ot_depth = 0;
    obj->active_state = 0;
    obj->vel_x = 0;
    obj->vel_y = 0;
    obj->state_0++;
    ref_other.p = obj->parent;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    a = 0x30;
    if (obj->facing_side == 0) {
        a = -0x30;
    }
    a += ref_other.p->pos_x;
    y = ref_other.p->pos_y;
    obj->pos_x = a;
    obj->pos_y = y - 0x46;
    set_vel_x_from_table_by_facing_slot04_0a(obj);
    if (ref_other.p->side == 0) {
        ((Slot04aObj *)obj)->scratchpad_value = *(int *)0x1f8000a8;
    } else {
        ((Slot04aObj *)obj)->scratchpad_value = *(int *)0x1f800158;
    }
    func_80138070(obj, obj->vel_table_index >> 1);
}

