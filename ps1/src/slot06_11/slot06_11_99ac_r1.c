/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801e99ac_slot06_11(Object *obj, int a) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->active_count += 1;
        ref_other.p->field_02 = 0x4b;
        ref_other.p->table_index = 1;
        ref_other.p->pos_x = (u16)obj->pos_x;
        ref_other.p->pos_y = (u16)obj->ground_y;
        ref_other.p->active_state = a;
    }
}

void func_801e9a64_slot06_11(Object *obj, int a) {
    ref_other.p = (Object *)func_8011f1e0();
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

void func_801e9b4c_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void func_801e9ba0_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    obj->facing_side = obj->facing_side ^ 1;
    ref_other.p = obj->parent;
    obj->pos_x = (u16)ref_other.p->pos_x;
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void func_801e9c1c_slot06_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 += 1;
    }
    obj->facing_side = obj->facing_side ^ 1;
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void func_801e9c7c_slot06_11(Object *obj) {
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

void func_801e9cfc_slot06_11(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
