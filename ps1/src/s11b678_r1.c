/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011b678(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (data_801ac888[i].active_flag != 0 && data_801ac888[i].enabled_flag != 0) {
            func_8011bc84(&data_801ac888[i]);
        }
    }
}
