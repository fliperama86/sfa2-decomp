/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014377c(Object *object) {
    u8 mode = game_state.config->mode;
    if (mode == 1) {
        object->field_01 = 1;
        object->health = 0x2a;
        object->countdown = 0x30;
        object->state_1 = object->state_1 + 1;
        object->sequence = seq_table_8017b50c[object->variant_index_2];
        object->step_timer = object->sequence->duration;
        object->step_flags = object->sequence->flags;
        if (object->variant_index_2 == 1) {
            object->state_1 = 7;
            object->health = 0;
            play_sound_at(0, 0, 0x205);
        }
        if (object->variant_index_2 == 0 && object->field_a4 == 1) {
            object->health = 0;
            object->state_1 = 7;
        }
    }
}
