/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80143c1c(Object *object) {
    s16 pos[4] = {0x1ca, 0x25, -0x4a, 0x25};
    if (object->field_a0 != 7) {
        if (object->field_a0 == 1 || (object->field_a0 == 0 && object->field_a4 == 1)) {
            if (object->field_a4 == 1) {
                func_80144f10(0, 7);
                func_80144f10(1, 7);
            }
            func_8011f240(object);
        } else {
            object->field_a0 = 7;
            object->health = 0x2a;
            object->field_46 = 0x30;
            object->field_0c = 1;
            object->sequence = seq_table_8017b50c[object->field_a0];
            object->step_timer = object->sequence->duration;
            object->step_flags = object->sequence->flags;
            object->pos_x = pos[object->field_03 * 2];
            object->pos_y = pos[object->field_03 * 2 + 1];
            object->state_1 = 1;
        }
    } else {
        game_state.config->field_6c = 4;
        func_8011f240(object);
    }
}
