/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern BoxTables *data_801be124_slot04_0c[];
extern BoxTables *data_801be150_slot04_0c[];
extern BoxTables *data_801be188_slot04_0c[];
extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);

void follow_parent_frame_param_sequence_slot04_0c(Object *obj, Object *parent);
void reset_and_push_block_object_type_8(Slab172 *o);

void clear_parent_link_and_set_sequence_6_slot04_0c(Object *obj) {
    obj->state_1++;
    obj->parent->linked_object_3 = 0;
    func_80138070(obj, 6);
}

void reset_states_on_negative_step_flags_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void func_801b3e88_slot04_0c(Object *o) {
    Object *p = o->parent;
    u8 n = p->link_flags - 1;
    p->link_flags = n;
    if (n == 0) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void set_box_tables_by_vel_index_and_health_slot04_0c(Object *obj) {
    if (obj->vel_table_index == 6) {
        obj->box_tables = data_801be124_slot04_0c[(s16)obj->health];
    } else if (obj->vel_table_index == 8) {
        obj->box_tables = data_801be150_slot04_0c[(s16)obj->health];
    } else {
        obj->box_tables = data_801be188_slot04_0c[(s16)obj->health];
    }
}

void call_handler_by_state_0_with_parent_slot04_0c(Object *obj) {
    data_801be1e0_slot04_0c[obj->state_0](obj, obj->parent);
}

void init_child_from_parent_fields_slot04_0c(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    follow_parent_frame_param_sequence_slot04_0c(obj, parent);
}
