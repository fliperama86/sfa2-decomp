/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define VOICES ((SndVoice *)0x801fc200)

void func_801193fc(int a) {
    VOICES[a].field_00 |= 0x10;
}

void func_80119420(int a) {
    VOICES[a].field_00 &= 0xffef;
}
