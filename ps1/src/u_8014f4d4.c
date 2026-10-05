/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f4d4(int a, s16 b) {
    Menu *m = &data_80190948;
    switch (a) {
    case 1:
        m->field_03 = 0;
        m->field_01 = 0;
        m->field_06 = 0;
        m->field_07 = 1;
        func_80150b80(m, b);
        break;
    case 2:
        m->field_06 = 1;
        func_8014f604(0);
        break;
    case 3:
        m->field_06 = 0;
        func_8014f604(0x55);
        break;
    case 4:
        m->field_02 = 4;
        m->field_03 = 0;
        break;
    case 5:
        m->field_03 = 1;
        m->field_1d = b;
        break;
    case 6:
        m->field_03 = 2;
        m->field_1d = b;
        break;
    }
}
