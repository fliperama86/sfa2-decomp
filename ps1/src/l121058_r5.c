/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_phase_by_pass_index(GameState *state) {
    if (data_80197fc4 != 0x701 && data_80198358 != 0x701) {
        if (((Attacker *)state)->phase != 0) {
            if (data_8018f5a0->pass_index == 0) {
                data_8018f5a0->pass_index++;
                scale_palette_rows_from_backup_bank(0, 0x20, 0xd, 0);
                scale_palette_rows_from_backup_bank(1, 0x20, 0xd, 0);
                scale_palette_rows_from_backup_bank(2, 0x20, 0xd, 0);
                scale_palette_rows_from_backup_bank(3, 0x20, 0xd, 0);
                queue_five_jobs();
                data_801ac6a8[0] = 0;
                ((Attacker *)state)->phase = 0x80;
            } else if (data_8018f5a0->pass_index == 1) {
                data_8018f5a0->pass_index++;
                load_command_8_for_player_kind_and_stage();
            } else {
                func_801e0020((Attacker *)state);
            }
        } else {
            data_8018f5a0->step_index_2 = 3;
            ((Attacker *)state)->field_4f = 0;
            ((Attacker *)state)->active_flag = 0;
            ((Attacker *)state)->field_2c = 0;
            data_801ac6a8[0] = 1;
        }
    }
}
