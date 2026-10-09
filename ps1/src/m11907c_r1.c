/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011907c(void) {
    for (data_8018f5a0 = (HudState *)0x801fc200; (u32)data_8018f5a0 <= 0x801fc37f; data_8018f5a0 = (HudState *)((SndVoice *)data_8018f5a0 + 1)) {
        switch (((SndVoice *)data_8018f5a0)->field_00) {
        case 3:
            func_801575dc();
            ((SndVoice *)data_8018f5a0)->field_04 = func_8015760c(((SndVoice *)data_8018f5a0)->field_0c, ((SndVoice *)data_8018f5a0)->field_08, ((SndVoice *)data_8018f5a0)->field_10);
            func_8015786c();
        case 2:
            ((SndVoice *)data_8018f5a0)->field_00 = 4;
            func_801577ec(((SndVoice *)data_8018f5a0)->field_04);
        }
    }
}
