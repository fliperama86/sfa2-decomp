/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_state_one_countdown_from_script(Object *object) {
    u16 value;
    object->field_208 = 0;
    object->field_209 = 0;
    value = *script_cursor++;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 0;
    object->countdown_3 = value;
}
