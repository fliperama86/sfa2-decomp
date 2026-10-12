/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011abe4(void) {
    int i;
    data_801a4fe8 = (u8 *)data_801987cc[data_801a27d0];
    data_801a697c = (u8 *)data_8018db18[data_801a27d0];
    for (i = 0; i < 40; i++) {
        if (data_801a89f4[i].active_flag != 0 && data_801a89f4[i].field_01 != 0) {
            func_8011bc84((Slab172 *)&data_801a89f4[i]);
        }
    }
}
