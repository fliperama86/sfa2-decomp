/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);

void increment_state_0_slot04_12(Object *obj);
void call_handler_by_input_dir_second_2_slot04_12(Object *obj);
void func_801b4310_slot04_12(Object *obj);
void check_range_hit_or_continue_sequence_slot04_12(Object *obj);
void update_sequence_or_finish_on_flag_slot04_12(Object *obj);
void func_801b4440_slot04_12(Object *obj);
void func_801b4480_slot04_12(Object *obj);
void update_sequence_or_finish_on_flag_2_slot04_12(Object *obj);
void func_801b4544_slot04_12(Object *obj);
void update_airborne_motion_or_land_slot04_12(Object *obj);
void func_801b4ba8_slot04_12(Object *obj);

void increment_state_0_slot04_12(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_and_clear_parent_link_slot04_12(Object *obj) {
    Object *p = obj->parent;

    if (obj->table_index == 0) {
        *(s32 *)&p->linked_object_2 = 0;
    } else {
        p->linked_object = 0;
    }
    func_8011f38c(obj);
}

void dispatch_by_input_dir_0_2_slot04_12(Object *obj) {
    if (obj->input_dir_0 == 4) {
        update_airborne_motion_or_land_slot04_12(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b4544_slot04_12(obj);
    } else {
        call_handler_by_input_dir_second_2_slot04_12(obj);
    }
}

void call_handler_by_input_dir_second_2_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b4440_slot04_12(obj);
    } else {
        func_801b4310_slot04_12(obj);
    }
}

void func_801b4310_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        check_range_hit_or_continue_sequence_slot04_12(obj);
    } else {
        update_sequence_or_finish_on_flag_slot04_12(obj);
    }
}

void check_range_hit_or_continue_sequence_slot04_12(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b4ba8_slot04_12(obj);
    }
}

void update_sequence_or_finish_on_flag_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4440_slot04_12(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b4480_slot04_12(obj);
    } else {
        update_sequence_or_finish_on_flag_2_slot04_12(obj);
    }
}
