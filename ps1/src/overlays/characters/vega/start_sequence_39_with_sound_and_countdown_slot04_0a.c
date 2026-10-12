/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c0688_slot04_0a[];
extern u8 data_801c068b_slot04_0a[];

void start_sequence_39_with_sound_and_countdown_slot04_0a(Object *obj) {
    int i;

    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    obj->field_29c = 2;
    obj->kind10_flag = 0xff;
    i = obj->direction_index >> 1;
    ((Slot04aObj *)obj)->countdown = data_801c0688_slot04_0a[i];
    obj->countdown = (s8)data_801c068b_slot04_0a[i];
    obj->field_225 = 1;
    obj->field_165 = 0;
    play_sound_at(obj, obj->side, 0x31c);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x39);
}
