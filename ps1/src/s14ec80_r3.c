/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f59c(void) {
    func_80150294(&ctl_80190948);
    func_80150758(&ctl_80190948);
}

void func_8014f5d4(u8 a) {
    ctl_80190948.field_04 = a;
    func_8014f604(ctl_80190948.field_05);
}

void func_8014f604(u8 a) {
    ctl_80190948.field_05 = a;
    if (ctl_80190948.field_04) {
        data_80189478[0] = a;
        data_80189478[1] = 0;
        data_80189478[2] = a;
        data_80189478[3] = 0;
    } else {
        data_80189478[0] = a >> 1;
        data_80189478[1] = a >> 1;
        data_80189478[2] = a >> 1;
        data_80189478[3] = a >> 1;
    }
    func_8015cd98(data_80189478);
}
