/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801374c0(void) {
    s16 i, j;

    if (data_8018db10) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 0x200; j++) {
                data_801a27e4_rows[i][j] = data_801a27e4_rows[i + 5][j];
            }
        }
        func_80137b10();
        data_8018db10 = 0;
    }
}
