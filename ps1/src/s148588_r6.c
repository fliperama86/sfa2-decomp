/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80148c8c(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cebc[object->state_0](object);
}

void init_object_motion_random_accel_y_and_sequence(Object *object) {
    object->state_0 = object->state_0 + 1;
    object->field_0c = 0;
    object->vel_x = *(s32 *)&object->pos_x_frac;
    object->vel_y = 0;
    object->accel_y = 0;
    object->pos_y = object->pos_y - 8;
    object->accel_y = get_random_byte() & 0x30;
    object->accel_x = 0;
    object->countdown = 0;
    set_sequence_from_table_5(object, object->variant_index, seqs_8017c7f8);
}
