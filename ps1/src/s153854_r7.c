/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void noop_object_handler_3(Object *o) {
}

void init_memory_card_events(void) {
    s32 *flag = (s32 *)0x1f800000;
    int *d = data_8018fef8;
    if (*flag < 0) {
        disable_and_close_events(d);
    }
    open_and_enable_events(d);
    if (*flag == 0) {
        func_8016d8e0(1);
        func_8016d8f0();
        func_8016d900();
    }
}
