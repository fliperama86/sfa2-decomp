/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_cd_control(void) {
    func_80150294(&ctl_80190948);
    call_cd_command_handler_by_state(&ctl_80190948);
}

void set_cd_mix_mode(u8 a) {
    ctl_80190948.cd_mix_mode = a;
    set_cd_volume(ctl_80190948.cd_volume);
}

void set_cd_volume(u8 a) {
    ctl_80190948.cd_volume = a;
    if (ctl_80190948.cd_mix_mode) {
        data_80189478[0] = a;
        data_80189478[1] = 0;
        data_80189478[2] = a;
        data_80189478[3] = 0;
    } else {
        data_80189478[0] = a >> 1;
        data_80189478[1] = a >> 1;
        data_80189478[2] = a >> 1;
        data_80189478[3] = a >> 1;
    }
    func_8015cd98(data_80189478);
}
