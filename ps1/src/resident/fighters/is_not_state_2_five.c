/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

int is_not_state_2_five(Object *object) {
    if (*(u16 *)&object->state_0 == 1) return object->state_2 != 5;
    return 1;
}
