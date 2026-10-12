/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_stream_control_by_mode(int a) {
    Ctl *ctl = &ctl_80190948;
    if (ctl_80190948.mode != 0) {
        if (a != 1) {
            ctl_80190948.field_1f++;
            close_slots_and_seek_cd_position(ctl);
        } else if (ctl_80190948.busy_flag == 0) {
            data_8017ec44[ctl_80190948.state_index](ctl);
            if (*(u32 *) ((u8 *) &ctl_80190948 + 0x30) == 0) {
                if (advance_stream_record((Stream *)ctl) == 0) {
                    pause_cd_and_set_stream_state_2((Stream *)ctl);
                }
            }
            ctl->field_09 = 0;
        }
    }
}
