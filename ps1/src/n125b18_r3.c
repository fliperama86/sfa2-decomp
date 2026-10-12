/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void start_fade_mode_1_and_wait(void) {
    while (game_state.wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
    game_state.wait_flag = 0x1f;
    *(u16 *)data_80190464 = 1;
    *(s16 *)data_8019046c = 0;
    set_voice_thread_entry_and_open(2, 4);
    while (game_state.fade_busy != 0) {
        request_sound_command_and_switch_thread(1);
    }
}

void start_fade_mode_c01_and_wait(void) {
    while (game_state.wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
    game_state.wait_flag = 0x1f;
    *(u16 *)data_80190464 = 0xc01;
    *(s16 *)data_8019046c = -1;
    set_voice_thread_entry_and_open(2, 4);
    while (game_state.fade_busy != 0) {
        request_sound_command_and_switch_thread(1);
    }
}

void start_fade_mode_1e01_no_wait(void) {
    if (game_state.fade_busy == 0) {
        game_state.wait_flag = 0x1f;
        *(u16 *)data_80190464 = 0x1e01;
        *(s16 *)data_8019046c = 0x1f00;
        set_voice_thread_entry_and_open(2, 4);
    }
}

void start_fade_mode_2001_no_wait(void) {
    if (game_state.fade_busy == 0) {
        game_state.wait_flag = 0x1f;
        *(u16 *)data_80190464 = 0x2001;
        *(s16 *)data_8019046c = 0x1f00;
        set_voice_thread_entry_and_open(2, 4);
    }
}

void start_fade_mode_1801_and_wait(void) {
    while (game_state.wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
    game_state.wait_flag = 0x1f;
    *(u16 *)data_80190464 = 0x1801;
    *(s16 *)data_8019046c = 0;
    set_voice_thread_entry_and_open(2, 4);
    while (game_state.fade_busy != 0) {
        request_sound_command_and_switch_thread(1);
    }
}

void start_fade_mode_1a01_and_wait(void) {
    while (game_state.wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
    game_state.wait_flag = 0x1f;
    *(u16 *)data_80190464 = 0x1a01;
    *(s16 *)data_8019046c = -1;
    set_voice_thread_entry_and_open(2, 4);
    while (game_state.fade_busy != 0) {
        request_sound_command_and_switch_thread(1);
    }
}

void start_fade_command_2a01_1f00_if_idle(void) {
    if (game_state.fade_busy == 0) {
        game_state.wait_flag = 0x1f;
        *(u16 *)data_80190464 = 0x2a01;
        *(s16 *)data_8019046c = 0x1f00;
        set_voice_thread_entry_and_open(2, 4);
    }
}
