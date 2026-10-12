/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

int is_pressed_bits_in_table(Object *object) {
    u16 v = object->pressed_bits;
    u16 i;
    if (v == 0xff || v == 0x95 || v == 0x6a) {
        goto zero;
    }
    for (i = 0; i < 0x18; i++) {
        if (v == table_8017ab88[i]) {
            return 1;
        }
    }
zero:
    return 0;
}
