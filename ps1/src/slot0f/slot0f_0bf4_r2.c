/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int check_memory_card_status_slot0f(int chan);

int get_usable_memory_card_mask_slot0f(void) {
    int result = 0;
    int status;

    status = check_memory_card_status_slot0f(0);
    if (status == 0 || status == 2 || status == 4) {
        result |= 1;
    }
    status = check_memory_card_status_slot0f(0x10);
    if (status == 0 || status == 2 || status == 4) {
        result |= 2;
    }
    return result;
}
