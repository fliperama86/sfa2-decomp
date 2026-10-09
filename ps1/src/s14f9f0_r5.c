/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80150758(Menu *menu) {
    if (menu->field_00 == 2 && menu->field_08 == 0) {
        data_8017ec7c[menu->field_02](menu);
        if (menu->field_03 != 0) {
            func_80150afc(menu);
        }
    }
}
