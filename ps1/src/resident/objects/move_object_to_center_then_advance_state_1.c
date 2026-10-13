/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void move_object_to_center_then_advance_state_1(Object *object) {
    object->pos_y = object->pos_y + 5;
    object->health = object->health - 3;
    if (object->table_index == 0) {
        object->pos_x = object->pos_x - 0x13;
        if (object->pos_x < 0xc1) {
            object->pos_x = 0xc0;
            object->pos_y = 0x70;
            object->state_1 = object->state_1 + 1;
            object->health = 0;
            object->image_row = 0;
            if (object->variant_index_2 == 7) {
                white_fill_active = 1;
            }
        }
    } else {
        object->pos_x = object->pos_x + 0x13;
        if (object->pos_x >= 0xc0) {
            object->pos_x = 0xc0;
            object->pos_y = 0x70;
            object->state_1 = object->state_1 + 1;
            object->health = 0;
            object->image_row = 0;
            if (object->variant_index_2 == 2) play_sound_at(0, 0, 0x200);
            if (object->variant_index_2 == 3) play_sound_at(0, 0, 0x201);
            if (object->variant_index_2 == 4) play_sound_at(0, 0, 0x202);
            if (object->variant_index_2 == 5) play_sound_at(0, 0, 0x203);
            if (object->variant_index_2 == 6) play_sound_at(0, 0, 0x204);
        }
    }
}
