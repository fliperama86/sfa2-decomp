/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void check_cd_position_and_set_command_state(void) {
    int code;
    int h;
    if ((*(u32 *)&data_80190948 & 0xff00ff) == 0x20002) {
        data_80190948.flag_b = 1;
        code = func_8015c814();
        if (code == 0x1b || code == 0x11) {
            if (func_8015c938(1, &data_80190948.cd_result) == 2 && !(data_80190948.cd_result & 0x10)) {
                if (code == 0x11) {
                    if (data_80190948.end_sector < func_8015cfc8(&data_80190948.field_11)) {
                        h = data_80190948.track_code;
                        code = (h & 0xff) + ((h & 0xff00) >> 5);
                        if (data_8017eb44[code] >= 0) {
                            data_80190948.command_state = 4;
                        } else {
                            data_80190948.command_state = 5;
                        }
                    }
                }
                func_8015cb08(0x11, 0);
            }
        }
    }
}
