/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2970_slot04_0a(Object *obj) {
    int a;
    int b;
    obj->field_225 = 1;
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    a = 0x80000;
    b = -0x4000;
    obj->active_state = 1;
    obj->field_165 = 0;
    obj->vel_y = 0x28000;
    obj->accel_y = b;
    if (obj->facing_side == 0) {
        a = 0xfff80000;
        b = 0x4000;
    }
    obj->vel_x = a;
    obj->accel_x = b;
    play_sound_at(obj, obj->side, 0x31c);
    spawn_children_per_direction_index(obj, -0x19, 0x44);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x30);
}
