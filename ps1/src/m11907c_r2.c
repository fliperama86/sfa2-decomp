/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define VOICES ((SndVoice *)0x801fc200)
extern u16 data_8019046c;

void func_801191dc(int a, u32 b) {
    VOICES[a].field_00 = 2;
    func_801575dc();
    VOICES[a].field_04 = func_8015760c(b, VOICES[a].field_08, VOICES[a].field_10);
    VOICES[a].field_68 = game_state.field_35c >> 8;
    VOICES[a].field_69 = *(u8 *)&game_state.field_35c;
    VOICES[a].field_6a = data_8019046c >> 8;
    VOICES[a].field_6b = data_8019047c;
    func_8015786c();
}
