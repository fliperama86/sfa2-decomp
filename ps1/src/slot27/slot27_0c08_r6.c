/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800111c8_slot27(void) {
    int i;
    int j;
    if (data_8018db10 != 0) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 0x200; j++) {
                data_801a27e4_rows[i][j] = data_801a27e4_rows[i + 5][j];
            }
        }
        func_80137220(0, 0);
        func_80137220(1, 1);
        func_80137220(2, 2);
        func_80137220(3, 3);
        func_80137220(4, 7);
        data_8018db10 = 0;
    }
}
