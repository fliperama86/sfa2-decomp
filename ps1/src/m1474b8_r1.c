/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void init_image_row_copy_with_sound_and_cd_volume(Object *object) {
    object->state_0 = object->state_0 + 1;
    data_80189458 = 0;
    data_8018945c = 0;
    object->variant_index_2 = 1;
    object->offset_index = 0x10;
    object->step_count = 0;
    if (game_state.field_64 != 0) {
        play_side_sound_at(object->parent, object->parent->side, 0xd);
        set_cd_volume(2);
        object->field_01 = 1;
        set_countdown_random_by_active_state(object);
        set_sequence_from_table_5(object, game_state.spawn_kind, seqs_8017cbac);
        store_image_row_and_queue_job(object, 1, 0x10);
    } else {
        object->state_0 = 2;
    }
}
