/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_cd_command_handler_by_state(Menu *menu) {
    if (menu->mode_index == 2 && menu->field_08 == 0) {
        data_8017ec7c[menu->command_state](menu);
        if (menu->fade_mode != 0) {
            update_cd_volume_fade(menu);
        }
    }
}
