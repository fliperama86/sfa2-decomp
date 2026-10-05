/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011a410(void) {
    int j, k;

    for (j = 0; j < 2; j++) {
        data_801900f8[j] = 0;
        data_801900f4[j] = 0;
        table_801aa4d8[j] = 0;
        for (k = 0; k < 32; k++) {
            table_801ac318[j][k].a = 0;
            table_801ac318[j][k].b = 0;
            table_801ac318[j][k].c = 0;
            table_801ac318[j][k].d = 0;
            table_801ac318[j][k].e = 0;
        }
    }
    for (j = 0; j < 2; j++) {
        for (k = 0; k < 4; k++) {
            data_80185c84[j][k][0] = 0;
            data_80185cc4[j][k][0] = 0;
            data_80185ce4[j][k][0] = 0;
            data_80185c84[j][k][1] = 0;
            data_80185cc4[j][k][1] = 0;
            data_80185ce4[j][k][1] = 0;
        }
    }
}
