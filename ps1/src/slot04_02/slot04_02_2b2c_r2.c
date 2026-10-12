/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_state_3_set_code_2_and_velocity_slot04_02(Object *obj) {
    obj->other_state_code = 2;
    *(s32 *)&obj->vel_x = 0x80000;
    obj->accel_x = -0x8000;
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x2a);
}

void update_spawn_effects_on_step_flag_code_2_slot04_02(Object *obj) {
    obj->other_state_code = 2;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x12, 0x30);
    }
    update_sequence(obj);
}
