/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8001336c_slot01(u8 arg) {
    int ret = 0;
    Object *p;
    u8 *flag = &data_801ae02c;

    if (*flag == 0) {
        *flag = 1;
        data_801ae02e = arg;
        data_801ae02d = 0;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            ret = 1;
            p->field_00 = 1;
            p->field_02 = 0x13;
            p->field_03 = arg + 0x80;
        }
    }
    return ret;
}
