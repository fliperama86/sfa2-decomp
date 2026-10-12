/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *pop_block_from_pool(void);

void spawn_object_4b_type1_at_ground_slot06_11(Object *obj, int a) {
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count += 1;
        ref_other.p->field_02 = 0x4b;
        ref_other.p->table_index = 1;
        ref_other.p->pos_x = (u16)obj->pos_x;
        ref_other.p->pos_y = (u16)obj->ground_y;
        ref_other.p->active_state = a;
    }
}

void spawn_object_4b_child_at_ground_slot06_11(Object *obj, int a) {
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count += 1;
        ref_other.p->field_02 = 0x4b;
        ref_other.p->table_index = 2;
        ref_other.p->table_index = a + ref_other.p->table_index;
        ref_other.p->pos_x = (u16)obj->pos_x;
        ref_other.p->pos_y = (u16)obj->ground_y;
        ref_other.p->facing_side = obj->side;
        ref_other.p->parent = obj;
    }
}

void advance_sequence_next_state_on_loop_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void advance_sequence_flip_side_copy_parent_x_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    obj->facing_side = obj->facing_side ^ 1;
    ref_other.p = obj->parent;
    obj->pos_x = (u16)ref_other.p->pos_x;
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void advance_sequence_and_flip_facing_side_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    obj->facing_side = obj->facing_side ^ 1;
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void advance_sequence_blink_by_frame_parity_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    advance_sequence_checked(obj);
    if (((game_state.frame_counter + obj->facing_side) & 1) != 0) {
        func_8011ffdc(obj);
    } else {
        obj->field_01 = 0;
    }
}

void release_object_record_3_slot06_11(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
