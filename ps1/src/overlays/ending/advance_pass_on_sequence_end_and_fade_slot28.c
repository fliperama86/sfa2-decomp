/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_800519d0_slot28;

void advance_pass_on_sequence_end_and_fade_slot28(Object *obj) {
    if ((s16)data_800519d0_slot28.p->step_flags < 0) {
        set_menu_command_by_code(1, 0x502);
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
