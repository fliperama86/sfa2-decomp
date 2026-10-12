/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void start_cd_track_from_table(Menu *m, int a, int b) {
    s32 v = data_8017e9d4[a][b];
    u8 *p;
    int i;
    if (m->mode_index == 2) {
        while (!func_8015cc44(9, 0, 0)) {
        }
    }
    m->mode_index = 1;
    m->retry_count = 6;
    m->track_code = v;
    m->field_15 = 0;
    m->field_16 = 0;
    m->field_17 = 0;
    m->pending_flag = 0;
    m->field_1f = 0;
    i = 3;
    p = (u8 *)m + 3;
    for (; i >= 0; i--) {
        p[0x20] = 0;
        p--;
    }
    set_sprite_fields_from_table(m, v);
    func_8015c990((int) update_stream_control_by_mode);
    seek_cd_position_with_retries(m);
}
