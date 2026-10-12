/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1cfc_slot04_03(Object *obj) {
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    obj->vel_x = 0x60000;
    obj->accel_x = -0x8000;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x32);
}

void func_801b1d70_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x18, 0x28);
    }
    update_sequence(obj);
}
