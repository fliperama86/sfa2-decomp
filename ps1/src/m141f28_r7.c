/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void start_spawned_object_sound_and_sequence(Object *object) {
    u8 phase;
    u8 key;
    data_80189458 = 0;
    data_8018945c = 0;
    phase = object->state_0;
    object->state_0 = phase + 1;
    if (game_state.field_64 != 0) {
        play_sound_at(0, 0, 0x32c);
        set_cd_volume(2);
        object->field_01 = 1;
        set_countdown_random_by_active_state(object);
        set_sequence_from_table_5(object, game_state.spawn_kind, table_8017cbac);
        key = object->active_state >> 1;
        if (key == 0) {
            bytes_8017cb18.first = 0;
            bytes_8017cb18.second = 0;
        } else if (key == 1) {
            bytes_8017cb18.first = 0;
            bytes_8017cb18.second = 0;
        } else if (key == 2) {
            bytes_8017cb18.first = 0;
            bytes_8017cb18.second = 0x10;
        } else if (key == 3) {
            bytes_8017cb18.first = 1;
            bytes_8017cb18.second = 0x10;
        }
        store_image_row_and_queue_job(object, bytes_8017cb18.first, bytes_8017cb18.second);
    } else {
        object->state_0 = phase + 3;
    }
}
