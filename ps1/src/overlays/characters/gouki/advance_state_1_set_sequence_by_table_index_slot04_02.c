/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_a_by_sound_side_slot04_02(Object *obj, u8 a);
void set_object_states_0_to_3_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void select_data_table_group_a_slot04_02(Object *obj, u8 a);
void select_data_table_group_b_slot04_02(Object *obj, u8 a);
void select_data_table_group_c_slot04_02(Object *obj, u8 a);
void select_data_table_group_d_slot04_02(Object *obj, u8 a);
extern void *data_801c7734_slot04_02[];
extern void *data_801c772c_slot04_02[];
extern void *data_801c7730_slot04_02[];
void set_data_table_ptr_from_table_slot04_02(Object *obj, void **tbl);

void advance_state_1_set_sequence_by_table_index_slot04_02(Object *obj) {
    int a = 0x10;

    obj->state_1++;
    if (obj->table_index != 2 && obj->table_index != 8) {
        a = 0xf;
    }
    set_sequence_a_by_sound_side_slot04_02(obj, a);
}

void reset_states_on_negative_step_flags_then_advance_slot04_02(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        set_object_states_0_to_3_3_slot04_02(obj, 3, 0, 0, 0);
    }
    advance_sequence_checked(obj);
}

void unlink_from_parent_and_release_slot04_02(Object *obj) {
    Object *p = obj->parent;
    u8 n = p->link_flags - 1;

    p->linked_object_3 = 0;
    p->link_flags = n;
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

int dispatch_table_pick_by_table_index_slot04_02(Object *obj, Object *parent) {
    u8 a = obj->vel_table_index;

    if (obj->table_index == 6) {
        select_data_table_group_d_slot04_02(obj, a);
        return 1;
    }
    if (obj->table_index == 8) {
        select_data_table_group_c_slot04_02(obj, a);
        return 1;
    }
    if (obj->table_index == 4) {
        select_data_table_group_b_slot04_02(obj, a);
        return 1;
    }
    if (obj->table_index == 2) {
        select_data_table_group_a_slot04_02(obj, a);
        return 1;
    }
    return -1;
}

void select_data_table_group_a_slot04_02(Object *obj, u8 a) {
    void **t = data_801c7734_slot04_02;

    if (a == 3) {
        t = data_801c772c_slot04_02;
    }
    if (a == 4) {
        t = data_801c7730_slot04_02;
    }
    set_data_table_ptr_from_table_slot04_02(obj, t);
}
