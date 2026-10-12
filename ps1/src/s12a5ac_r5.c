/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: RESIDUAL update_airborne_motion_simple_or_land: original hoists "move a0,s0" above the first beq and uses a0 in the negative arm; built keeps it in the bltz slot (+4 bytes). Nested-if and merged-call forms tried. */



void func_8012b0e4(Object *object) {
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 3;
    object->state_3 = 0;
    object->once_flag = 0;
    object->facing_side = object->saved_facing;
    object->variant_index = object->direction_flag;
    set_motion_from_table(object);
    set_sequence_by_side(object, 0x10);
}

void func_8012b144(Object *object) {
    object->field_16a = 0;
    handler_table_19d0[object->state_3](object);
}
