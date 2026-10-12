/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void switch_to_variant_7_or_release_object(Object *object) {
    s16 pos[4] = {0x1ca, 0x25, -0x4a, 0x25};
    if (object->variant_index_2 != 7) {
        if (object->variant_index_2 == 1 || (object->variant_index_2 == 0 && object->field_a4 == 1)) {
            if (object->field_a4 == 1) {
                spawn_object_with_position_table(0, 7);
                spawn_object_with_position_table(1, 7);
            }
            reset_and_push_object_to_pool_table(object);
        } else {
            object->variant_index_2 = 7;
            object->health = 0x2a;
            object->countdown = 0x30;
            object->field_0c = 1;
            object->sequence = seq_table_8017b50c[object->variant_index_2];
            object->step_timer = object->sequence->duration;
            object->step_flags = object->sequence->flags;
            object->pos_x = pos[object->table_index * 2];
            object->pos_y = pos[object->table_index * 2 + 1];
            object->state_1 = 1;
        }
    } else {
        game_state.config->mode = 4;
        reset_and_push_object_to_pool_table(object);
    }
}
