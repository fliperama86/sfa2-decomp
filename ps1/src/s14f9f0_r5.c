/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80150758(Menu *menu) {
    if (menu->mode_index == 2 && menu->field_08 == 0) {
        data_8017ec7c[menu->command_state](menu);
        if (menu->fade_mode != 0) {
            update_cd_volume_fade(menu);
        }
    }
}
