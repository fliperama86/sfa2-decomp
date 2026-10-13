/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void clear_four_144_byte_arrays(void) {
    u8 *a = background_layers;
    u8 *b = main_layer;
    u8 *c = cam_obj;
    u8 *d = fourth_layer;
    unsigned int i;
    int z = 0;
    for (i = 0; i < 0x90; i++) {
        *a++ = z;
        *b++ = z;
        *c++ = z;
        *d++ = z;
    }
}
