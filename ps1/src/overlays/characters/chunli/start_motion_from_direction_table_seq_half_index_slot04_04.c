/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c4344_slot04_04[];

void start_motion_from_direction_table_seq_half_index_slot04_04(Object *obj) {
    u16 a;

    obj->state_3++;
    obj->countdown = *(u8 *)&obj->countdown + 0x400;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    obj->vel_x = data_801c4344_slot04_04[obj->direction_index * 2];
    obj->accel_x = data_801c4344_slot04_04[obj->direction_index * 2 + 1];
    obj->vel_y = data_801c4344_slot04_04[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c4344_slot04_04[obj->direction_index * 2 + 3];
    a = (obj->direction_index >> 1) + 0x50;
    set_sequence_by_side_and_mode(obj, a);
}

void count_down_high_byte_then_spawn_children_slot04_04(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        obj->countdown = *(u8 *)&obj->countdown + 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -6, 0x4a);
    }
}
