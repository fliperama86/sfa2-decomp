/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011d5b8(Slab172 *p, u32 q) {
    u8 i = p->field_02;
    u16 k;
    u8 b;
    u32 j;
    k = p->field_94;
    func_8011a6b8(data_80183d9c[i][9], data_80183dc4[i][9]);
    for (j = 10; j >= 2; j--) {
        data_80183d74[i][j - 1] = data_80183d74[i][j - 2];
        data_80183d9c[i][j - 1] = data_80183d9c[i][j - 2];
        data_80183dc4[i][j - 1] = data_80183dc4[i][j - 2];
        data_80183dec[i][j - 1] = data_80183dec[i][j - 2];
    }
    b = p->field_0b;
    data_80183dc4[i][1] = 0;
    data_80183dec[i][0] = q;
    data_80183d9c[i][0] = k;
    data_80183910[k] = k;
    data_801839f0[k] = 0;
    data_80183ad0[k] = 0;
    data_80183bb0[k] = 0;
    data_80183d74[i][0] = b;
}
