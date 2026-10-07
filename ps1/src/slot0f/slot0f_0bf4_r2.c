/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_800e0da8_slot0f(int chan);

int func_800e0d30_slot0f(void) {
    int result = 0;
    int status;

    status = func_800e0da8_slot0f(0);
    if (status == 0 || status == 2 || status == 4) {
        result |= 1;
    }
    status = func_800e0da8_slot0f(0x10);
    if (status == 0 || status == 2 || status == 4) {
        result |= 2;
    }
    return result;
}
