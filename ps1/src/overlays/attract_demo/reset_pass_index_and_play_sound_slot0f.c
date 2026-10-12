/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern int data_800f8578_slot0f;

void reset_pass_index_and_play_sound_slot0f(void) {
    data_800f8578_slot0f = 0;
    data_8018f5a0->pass_index = 0;
    play_sound_at(0, 0, 0x205);
}
