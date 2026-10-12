/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b21c8_slot04_05(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    if (obj->facing_side != 0) {
        obj->vel_x = 0x50000;
    } else {
        obj->vel_x = -0x50000;
    }
    obj->accel_x = 0;
    obj->vel_y = 0x88000;
    obj->accel_y = -0x9000;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x29);
}

void spawn_pooled_objects_on_step_flag_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x10, 0x43);
    }
    update_sequence(obj);
}
