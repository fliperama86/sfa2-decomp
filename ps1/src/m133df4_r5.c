/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801a27e4[];

void func_80137564(void) {
    s16 i, j, k;
    u16 *p;

    if (data_8018db10 == 0) {
        for (i = 0; i < 5; i++) {
            j = 1;
            p = (u16 *)(data_801a27e4 + (i << 10));
            for (; j < 0x10; j++) {
                for (k = 0; k < 0x20; k++) {
                    p[(k << 4) + j] = 0;
                }
            }
        }
        data_8018f598 = 1;
        data_8018db10 = data_8018db10 + 1;
    }
    func_80137b10();
}
