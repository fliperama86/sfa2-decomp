/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int init_cd_menu(void) {
    Menu *m = &data_80190948;
    u8 buf[4];
    int i = 4;
    int ok = 0;
    buf[0] = 0x80;
    for (; i != 0; i--) {
        if (func_8015c6ec()) {
            ok = 1;
            break;
        }
    }
    set_cd_volume(0);
    m->mode_index = 0;
    m->active_flag = 0;
    m->command_state = 0;
    m->fade_mode = 0;
    m->flag_a = 0;
    m->field_08 = 0;
    if (ok) {
        while (!func_8015cc44(0xe, (int)buf, 0)) {
        }
        m->cd_sector = 0x10;
        func_8015cec4(0x10, &m->cd_position);
        while (!func_8015cc44(0x15, (int)&m->cd_position, 0)) {
        }
    }
    return ok;
}
