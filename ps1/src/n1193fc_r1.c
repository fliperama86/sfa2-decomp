/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define VOICES ((SndVoice *)0x801fc200)

void set_voice_status_bit(int a) {
    VOICES[a].status |= 0x10;
}

void clear_voice_status_bit(int a) {
    VOICES[a].status &= 0xffef;
}
