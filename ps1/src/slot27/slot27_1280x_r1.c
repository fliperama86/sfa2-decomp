/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


void func_80011280_slot27(void) {
    int set;
    int i;
    int j;
    int r;
    int g;
    int b;
    int v;
    int off;
    int k = 0x10;


    if (data_8018db10 == 0) {
        for (set = 0; set < 5; set++) {
            for (i = 0; i < 0x20; i++) {
                for (j = 1; j < 0x10; j++) {
                    off = i * 0x10 + j;
                    v = data_801a27e4_rows[set + 5][off];
                    r = (v >> 10) & 0x1f;
                    g = (v >> 5) & 0x1f;
                    b = v & 0x1f;
                    r = (r * k) / 0x20 + 1;
                    g = (g * k) / 0x20 + 1;
                    b = (b * k) / 0x20 + 1;
                    v = (r << 10) | (g << 5) | b;
                    data_801a27e4_rows[set][off] = v;
                }
            }
        }
        func_80137220(0, 0);
        func_80137220(1, 1);
        func_80137220(2, 2);
        func_80137220(3, 3);
        func_80137220(4, 7);
        data_8018db10 = data_8018db10 + 1;
    }
}
