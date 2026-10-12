/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void init_spawned_object_with_random_offset(Object *object) {
    u8 facing;
    int hi;

    object->field_01 = 1;
    object->spawn_flag = 1;
    object->data_ptr_a = &data_80172a48;
    object->state_0 = object->state_0 + 1;
    object->field_90 = (void *)0x800fb100;
    object->ot_depth = 0;
    object->data_ptr_b = &data_80173c9c;
    object->pos_x = object->pos_x + table_8017c9f0[get_random_byte() & 0xf];
    object->pos_y = object->pos_y - table_8017c9f0[get_random_byte() & 0xf];
    set_image_row_from_team_id(object);
    object->facing_side = object->parent->facing_side;
    facing = object->table_index;
    if ((facing & 0x80) == 0) {
        hi = object->variant_index != 0;
        set_sequence_from_table_5(object, table_8017ca10[(hi << 3) + (facing & 7)], table_8017c7f8);
    } else {
        object->ot_depth = 8;
        follow_parent_position_with_frame_offset(object);
        set_sequence_from_table_5(object, table_8017ca20[facing & 7], table_8017c7f8);
    }
}
