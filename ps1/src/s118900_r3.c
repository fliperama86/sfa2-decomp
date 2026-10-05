/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define VOICES ((SndVoice *)0x801fc200)
extern SndCtl *data_8018f5a0;

void func_80119144(int a, int b) {
    func_8011e938(&VOICES[a].field_0c, b);
    func_801191dc(a, VOICES[a].field_0c);
}

void func_80119198(int a) {
    func_8011e938(&data_8018f5a0->field_0c, a);
    func_801193a4(data_8018f5a0->field_0c);
}
