/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void move_object_left_until_threshold(Object *object) {
    object->pos_x -= 9;
    object->health = 0;
    object->pos_y = 0x70;
    if (object->pos_x < 0xc1) {
        object->state_1 = object->state_1 + 1;
        if (object->variant_index_2 == 1) spawn_object_with_position_table(2, 0);
    }
}
