/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * written for readability and its build differs from the original's bytes.
 * The build does not use this file. The differential test next to it
 * (difftest.py, with func_8011a018.py) compares the behavior of this C with
 * the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): initializes the tables of
 * a two-buffer drawing system. It clears three small tables of 2 x 10
 * entries, clears seven tables of 0x6f entries plus one terminating entry
 * (the table data_80183c90 links each entry to the next, index + 1, and ends
 * with 0xffff in the terminating entry), sets data_801846fc to 1 and
 * data_80184700 to 0x6f. Then, for each of the 2 x 0x500 cells of the cell
 * buffers (the two buffers of data_801987cc, cell index outer, buffer inner),
 * it calls func_8015c0ec on the cell, sets bytes 4 to 6 to 0x80 and stores
 * into the halfword at 0xe the result of func_8015bdd4(0x60, 0x1e0). Last it
 * initializes with func_80158a2c the 2 x 0x70 primitives of data_8018db18
 * and the 2 x 0x28 of data_8019056c, each with 1, 1, a value from
 * func_8015bd0c(0, 0, (index / 16) * 64 + 0x180, 0) and 0.
 *
 * Contract:
 *   No argument, no result. Reads nothing but the results of the callees.
 *   Writes: the tables above, the cell buffers (bytes 4 to 6 and the
 *     halfword at 0xe of each cell), and nothing else.
 *   Callees replaced by recorders in the test (all lie in Sony's library,
 *     from 0x80157090 on): func_8015c0ec (1 argument), func_8015bdd4 (2),
 *     func_8015bd0c (4), func_80158a2c (5; the fifth is read from the
 *     stack). func_8015bdd4 and func_8015bd0c return one random value
 *     per case, the others 0. What they would do themselves is outside the
 *     test; the order and the arguments of the calls are compared. Watched
 *     at every call: data_801846fc and data_80184700 and the last entry of
 *     three of the tables; func_8015c0ec records the 16 bytes of its cell.
 *     The cell buffers are too large to watch whole, and the stores into a
 *     cell between its func_8015c0ec call and the next call are therefore
 *     not ordered against func_8015bdd4 (which gets no pointer).
 *   Aliasing: all tables and buffers are distinct.
 *   Excluded inputs: none. Not reached by any input: none (no branch but
 *     loops of fixed length).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8015c0ec(void *cell);
int func_8015bdd4(int a, int b);

void func_8011a018(void) {
    Cell16 *cells = (Cell16 *)data_801987cc;
    Prim *prims_a = (Prim *)data_8018db18;
    Prim *prims_b = (Prim *)data_8019056c;
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
    data_80183ad0[0x6f] = 0;
    data_80183bb0[0x6f] = 0;
    data_801839f0[0x6f] = 0;
    data_80183910[0x6f] = 0;
    data_801841bc[0x6f][0] = 0;
    data_801841bc[0x6f][1] = 0;
    data_80183ffc[0x6f][0] = 0;
    data_80183ffc[0x6f][1] = 0;
    data_80183c90[0x6f] = 0xffff;
    data_801846fc = 1;
    data_80184700 = 0x6f;

    for (i = 0; i < 0x500; i++) {
        for (j = 0; j < 2; j++) {
            Cell16 *c = &cells[j * 0x500 + i];
            func_8015c0ec(c);
            c->field_04 = 0x80;
            c->field_05 = 0x80;
            c->field_06 = 0x80;
            c->field_0e = func_8015bdd4(0x60, 0x1e0);
        }
    }
    for (i = 0; i < 0x70; i++) {
        for (j = 0; j < 2; j++) {
            func_80158a2c(&prims_a[j * 0x70 + i], 1, 1, func_8015bd0c(0, 0, (i / 16) * 64 + 0x180, 0), 0);
        }
    }
    for (i = 0; i < 0x28; i++) {
        for (j = 0; j < 2; j++) {
            func_80158a2c(&prims_b[j * 0x28 + i], 1, 1, func_8015bd0c(0, 0, (i / 16) * 64 + 0x180, 0), 0);
        }
    }
}
