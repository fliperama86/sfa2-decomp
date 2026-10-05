/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011cdc0(Slab172 *p) {
    u8 i = p->field_02;
    u32 j;
    if (data_80183d74[i][0] == data_80183d74[i][1]
        && data_80183d9c[i][0] == data_80183d9c[i][1]
        && data_80183dc4[i][1] == 0
        && data_80183dec[i][0] == data_80183dec[i][1]) {
        data_80183dc4[i][1] = data_80183dc4[i][0];
    } else {
        func_8011a6b8(data_80183d9c[i][0], data_80183dc4[i][0]);
    }
    for (j = 0; j < 9; j++) {
        data_80183d74[i][j] = data_80183d74[i][j + 1];
        data_80183d9c[i][j] = data_80183d9c[i][j + 1];
        data_80183dc4[i][j] = data_80183dc4[i][j + 1];
        data_80183dec[i][j] = data_80183dec[i][j + 1];
    }
    data_80183dc4[i][j] = 0;
}
