/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void start_pending_snd_voice_threads(void) {
    for (screen_state = (HudState *)0x801fc200; (u32)screen_state <= 0x801fc37f; screen_state = (HudState *)((SndVoice *)screen_state + 1)) {
        switch (((SndVoice *)screen_state)->status) {
        case 3:
            func_801575dc();
            ((SndVoice *)screen_state)->thread_handle = func_8015760c(((SndVoice *)screen_state)->thread_entry, ((SndVoice *)screen_state)->stack_address, ((SndVoice *)screen_state)->thread_arg);
            func_8015786c();
        case 2:
            ((SndVoice *)screen_state)->status = 4;
            func_801577ec(((SndVoice *)screen_state)->thread_handle);
        }
    }
}
