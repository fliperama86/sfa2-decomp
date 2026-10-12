/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_countdown_random_multiple_of_4_slot06_0f(Object *obj);

void set_countdown_random_multiple_of_4_slot06_0f(Object *obj) {
    obj->countdown = get_random_byte() & 0x3c;
}
