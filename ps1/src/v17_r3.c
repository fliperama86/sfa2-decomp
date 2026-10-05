/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Block172 *ref_third;

void func_8011f010(void) {
    u8 *a = data_801aa544;
    u8 *b = data_801aa5d4;
    u8 *c = cam_obj;
    u8 *d = data_801904d8;
    unsigned int i;
    int z = 0;
    for (i = 0; i < 0x90; i++) {
        *a++ = z;
        *b++ = z;
        *c++ = z;
        *d++ = z;
    }
}
