/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void set_move_state_dir_0_with_countdown(Object *object) {
    u16 value = *data_80189460++;
    data_80189464 = value;
    object->countdown_3 = value;
    object->field_208 = 1;
    object->field_209 = 3;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 2;
    object->state_3 = 0;
    object->variant_index = 0;
    object->direction_flag = 0;
}
