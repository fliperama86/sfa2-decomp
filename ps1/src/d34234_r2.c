/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_8015c0ec(void *p);

void func_8011a018(void) {
    unsigned i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++) {
            data_80183d9c[i][j] = 0;
            data_80183dc4[i][j] = 0;
            data_80183dec[i][j] = 0;
        }
    }
    for (i = 0; i < 0x6f; i++) {
        data_80183ad0[i] = 0;
        data_80183bb0[i] = 0;
        data_801839f0[i] = 0;
        data_80183910[i] = 0;
        data_801841bc[i][0] = 0;
        data_801841bc[i][1] = 0;
        data_80183ffc[i][0] = 0;
        data_80183ffc[i][1] = 0;
        data_80183c90[i] = i + 1;
    }
    data_80183ad0[i] = 0;
    data_80183bb0[i] = 0;
    data_801839f0[i] = 0;
    data_80183910[i] = 0;
    data_801841bc[i][0] = 0;
    data_801841bc[i][1] = 0;
    data_80183ffc[i][0] = 0;
    data_80183ffc[i][1] = 0;
    data_80183c90[i] = 0xffff;
    data_801846fc = 1;
    data_80184700 = 0x6f;
    for (i = 0; i < 0x280; i++) {
        for (j = 0; j < 2; j++) {
            Cell16 *c;
            func_8015c0ec(&data_801987cc[j][i]);
            /* The cell's address is formed a second time, as a sum of bytes: the original keeps one address for the call and another for the three stores. Written as &data_801987cc[j][i] here too, the function is 12 bytes shorter and differs in 18 instruction slots. */
            c = (Cell16 *)((u8 *)data_801987cc + (i * 16 + j * 0x5000));
            c->field_04 = 0x80;
            c->field_05 = 0x80;
            c->field_06 = 0x80;
            data_801987cc[j][i].field_0e = func_8015bdd4(0x60, 0x1e0);
        }
    }
    for (i = 0; i < 0x280; i++) {
        for (j = 0; j < 2; j++) {
            Cell16 *c;
            /* The second half of each buffer, addressed in bytes from its start as the original does: with a Cell16 pointer and sums of cells the function differs in 22 instruction slots, with &data_801987cc[j][0x280 + i] in 62. */
            u8 *b = (u8 *)&data_801987cc[0][0x280];
            func_8015c0ec(b + j * 0x5000 + i * 16);
            c = (Cell16 *)(b + (i * 16 + j * 0x5000));
            c->field_04 = 0x80;
            c->field_05 = 0x80;
            c->field_06 = 0x80;
            data_801987cc[j][0x280 + i].field_0e = func_8015bdd4(0x60, 0x1e0);
        }
    }
    for (i = 0; i < 0x70; i++) {
        for (j = 0; j < 2; j++) {
            func_80158a2c(&data_8018db18[j][i], 1, 1, func_8015bd0c(0, 0, (i / 16) * 64 + 0x180, 0), 0);
        }
    }
    for (i = 0; i < 0x28; i++) {
        for (j = 0; j < 2; j++) {
            func_80158a2c(&data_8019056c[j][i], 1, 1, func_8015bd0c(0, 0, (i / 16) * 64 + 0x180, 0), 0);
        }
    }
}
