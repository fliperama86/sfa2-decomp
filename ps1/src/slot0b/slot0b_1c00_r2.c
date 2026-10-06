/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801e1d0c_slot0b(u8 arg) {
    u8 *p = &data_801ae02c;
    if (*p == 0) {
        *p = 1;
        *(s16 *)(&data_801ae02c + 0x7a) = 0x80;
        *(s16 *)(&data_801ae02c + 0x7c) = 0x1e0;
        (&data_801ae02c)[2] = arg;
        (&data_801ae02c)[0xd] = 0x1b;
        (&data_801ae02c)[1] = 0;
        return 1;
    }
    return 0;
}
