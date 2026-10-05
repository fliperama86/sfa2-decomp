/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8016cc18(unsigned a) {
    int i;
    WordPair *e;

    for (i = 0; i < data_80183600; i++) {
        e = data_80183608 + i;
        if (e->field_00 & 0x40000000) {
            break;
        }
        if (e->field_00 == a) {
            e->field_00 = a | 0x80000000;
            break;
        }
    }
    func_8016d4cc();
}

void func_8016cc94(void) {
    func_8016cd84(0);
}
