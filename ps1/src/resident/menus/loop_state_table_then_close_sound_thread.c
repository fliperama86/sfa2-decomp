/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void loop_state_table_then_close_sound_thread(void) {
    GameState *g = &game_state;
    screen_state->state_index = 0;
    game_state.step_counter = 0;
    while (g->loop_mode != 0 || g->wait_flag != 0) {
        table_8017f2d8[screen_state->state_index](g);
        add_item_to_table(&data_8017f2ec);
        request_sound_command_and_switch_thread(1);
    }
    scale_palette_rows_from_backup_bank(4, 0x20, 0x1f, 0);
    queue_image_upload_job(4, 7);
    close_snd_thread_and_switch_main();
}
