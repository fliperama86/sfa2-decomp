/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801bfe9c_slot04_00[];

void start_upward_launch_state_3_slot04_00(Object *obj) {
    obj->state_3++;
    obj->vel_y = 0x40000;
    obj->accel_y = -0x6000;
    obj->vel_x = 0;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    func_80145d20(obj);
    set_sequence_by_side_and_mode(obj, 0x28);
}

void func_801b2438_slot04_00(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        spawn_two_offset_objects_at_object(obj);
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0x28, 0x3c);
    }
    update_sequence(obj);
}

void func_801b24c0_slot04_00(Object *obj) {
    int i = 0;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 9);
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801bfe9c_slot04_00[i];
    }
}
