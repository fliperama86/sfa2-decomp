/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ad8_slot28[];

void advance_pass_when_object_state_3_is_1_slot28(Object *obj) {
    if (data_80051ad8_slot28[0]->state_3 == 1) {
        data_8018f5a0->pass_index++;
        start_fade_command_2a01_1f00_if_idle();
    }
}
