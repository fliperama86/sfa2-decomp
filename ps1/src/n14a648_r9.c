/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern void (*data_8017ec44[])(Ctl *ctl);
void func_80150244(Ctl *ctl);

void func_8014f918(int a) {
    Ctl *ctl = &ctl_80190948;
    if (ctl_80190948.field_0a != 0) {
        if (a != 1) {
            ctl_80190948.field_1f++;
            func_8014f76c(ctl);
        } else if (ctl_80190948.field_14 == 0) {
            data_8017ec44[ctl_80190948.field_0b](ctl);
            if (*(u32 *) ((u8 *) &ctl_80190948 + 0x30) == 0) {
                if (func_801501a0((Stream *)ctl) == 0) {
                    func_80150244(ctl);
                }
            }
            ctl->field_09 = 0;
        }
    }
}
