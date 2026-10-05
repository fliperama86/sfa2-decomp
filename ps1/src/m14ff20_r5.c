/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801510bc(void) {
    int i;
    Block172 *b = data_801ac888;
    for (i = 0; i < 16; b++, i++) {
        if (b->field_00 != 0) {
            if (b->field_66 == 0) {
                table_8017f2fc[b->field_02](b);
            } else {
                table_8017f3a0[b->field_02](b);
            }
        }
    }
}
