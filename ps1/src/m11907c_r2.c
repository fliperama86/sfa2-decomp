/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define VOICES ((SndVoice *)0x801fc200)

void open_snd_voice_thread(int a, u32 b) {
    VOICES[a].status = 2;
    func_801575dc();
    VOICES[a].thread_handle = func_8015760c(b, VOICES[a].stack_address, VOICES[a].thread_arg);
    VOICES[a].saved_high_byte = game_state.step_value >> 8;
    VOICES[a].saved_low_byte = *(u8 *)&game_state.step_value;
    VOICES[a].saved_byte_a = *(u16 *)data_8019046c >> 8;
    VOICES[a].saved_byte_b = data_8019047c;
    func_8015786c();
}
