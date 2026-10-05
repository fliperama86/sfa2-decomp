/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011be18(Slab172 *p) {
    u16 i = p->field_94;
    func_8011a6b8(data_80183ffc[i][0], data_801841bc[i][0]);
    data_80183ffc[i][0] = data_80183ffc[i][1];
    data_801841bc[i][0] = data_801841bc[i][1];
    data_8018437c[i][0] = data_8018437c[i][1];
    data_801841bc[i][1] = 0;
    if (data_80183ffc[i][0] == 0 && data_80183ffc[i][1] == 0) {
        p->field_94 = 0;
    }
    if (data_801841bc[i][0] == 0 && data_801841bc[i][1] == 0) {
        p->field_94 = 0;
    }
}
