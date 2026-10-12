/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void start_pending_snd_voice_threads(void) {
    for (data_8018f5a0 = (HudState *)0x801fc200; (u32)data_8018f5a0 <= 0x801fc37f; data_8018f5a0 = (HudState *)((SndVoice *)data_8018f5a0 + 1)) {
        switch (((SndVoice *)data_8018f5a0)->status) {
        case 3:
            func_801575dc();
            ((SndVoice *)data_8018f5a0)->thread_handle = func_8015760c(((SndVoice *)data_8018f5a0)->thread_entry, ((SndVoice *)data_8018f5a0)->stack_address, ((SndVoice *)data_8018f5a0)->thread_arg);
            func_8015786c();
        case 2:
            ((SndVoice *)data_8018f5a0)->status = 4;
            func_801577ec(((SndVoice *)data_8018f5a0)->thread_handle);
        }
    }
}
