/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_800110b8_slot27(void) {
    Object *o;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0xa9;
        o->field_03 = 0;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0xab;
        o->field_03 = 0;
    }
}

void func_80011118_slot27(void) {
    int i;
    int j;
    u16 *row;
    if (data_8018db10 == 0) {
        for (i = 0; i < 5; i++) {
            row = data_801a27e4_rows[i];
            for (j = 0x1ff; j >= 0; j--) {
                row[j] = 0x7fff;
            }
        }
        func_80137220(0, 0);
        func_80137220(1, 1);
        func_80137220(2, 2);
        func_80137220(3, 3);
        func_80137220(4, 7);
        data_8018db10++;
    }
}
