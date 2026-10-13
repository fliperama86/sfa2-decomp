/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_start_slot_object_slot0b(u8 arg) {
    u8 *p = &single_object_active;
    if (*p == 0) {
        *p = 1;
        *(s16 *)(&single_object_active + 0x7a) = 0x80;
        *(s16 *)(&single_object_active + 0x7c) = 0x1e0;
        (&single_object_active)[2] = arg;
        (&single_object_active)[0xd] = 0x1b;
        (&single_object_active)[1] = 0;
        return 1;
    }
    return 0;
}
