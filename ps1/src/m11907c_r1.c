/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern SndVoice *data_8018f5a0;

void func_8011907c(void) {
    for (data_8018f5a0 = (SndVoice *)0x801fc200; (u32)data_8018f5a0 <= 0x801fc37f; data_8018f5a0++) {
        switch (data_8018f5a0->field_00) {
        case 3:
            func_801575dc();
            data_8018f5a0->field_04 = func_8015760c(data_8018f5a0->field_0c, data_8018f5a0->field_08, data_8018f5a0->field_10);
            func_8015786c();
        case 2:
            data_8018f5a0->field_00 = 4;
            func_801577ec(data_8018f5a0->field_04);
        }
    }
}
