/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

#define VOICES ((SndVoice *)0x801fc200)

void set_voice_thread_entry_and_open(int a, int b) {
    set_table_entry_and_global_pointer(&VOICES[a].thread_entry, b);
    open_snd_voice_thread(a, VOICES[a].thread_entry);
}

void set_snd_ctl_thread_entry_and_close(int a) {
    set_table_entry_and_global_pointer(&((SndCtl *)screen_state)->field_0c, a);
    close_sound_thread_and_switch_to_main(((SndCtl *)screen_state)->field_0c);
}
