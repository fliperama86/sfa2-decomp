/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void load_table_entry_or_wait_then_close_vab(GameState *state) {
    if (*(u32 *)0x1f80000c == 0) {
        if (*(u32 *)0x1f800010 != 0) {
            u32 v = table_801802dc[*(u32 *)0x1f800014];
            u32 lo;
            data_801ac624 = v;
            lo = v & 0xffff;
            state->kind_index = v >> 16;
            data_801a8067 = data_801a83fb = lo;
            load_kind_images_to_vram(v >> 16);
            load_player_sound_banks(lo, lo);
        } else {
            set_voice_thread_entry_and_open(2, 7);
            request_sound_command_and_switch_thread(1);
        }
    } else {
        set_voice_thread_entry_and_open(2, 7);
        do {
            request_sound_command_and_switch_thread(1);
        } while (!(state->wait_flag & 0x80));
    }
    close_vab_and_sep(2);
    start_cd_track_wait_with_thread_switch(4, 0);
}

void reset_side_entries(void) {
    data_8018d210[0].kind = 0;
    data_8018d210[0].side = 0;
    data_8018d210[0].active_flag = 0;
    data_8018d210[0].state_index = 0;
    data_8018d210[0].field_04 = 0;
    data_8018d210[0].substep_index = 0;
    data_8018d210[0].current_count = 0;
    data_8018d210[0].table_index = 0;
    data_8018d210[0].field_09 = 0;
    data_8018d210[0].field_0b = 0;
    data_8018d210[0].countdown_timer = 0;
    data_8018d210[0].field_10 = 0;
    data_8018d210[1].kind = 0;
    data_8018d210[1].side = 1;
    data_8018d210[1].active_flag = 0;
    data_8018d210[1].state_index = 0;
    data_8018d210[1].field_04 = 0;
    data_8018d210[1].substep_index = 0;
    data_8018d210[1].current_count = 0;
    data_8018d210[1].table_index = 0;
    data_8018d210[1].field_09 = 0;
    data_8018d210[1].field_0b = 0;
    data_8018d210[1].countdown_timer = 0;
    data_8018d210[1].field_10 = 0;
}
