/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4bc4_slot04_04(Object *object) {
    object->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(object);
}
