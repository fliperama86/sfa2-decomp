/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int test_input_mask_with_direction_slot04_0f(Object *obj, u16 *mask, BytePair dir);

int test_input_mask_with_direction_slot04_0f(Object *obj, u16 *mask, BytePair dir) {
    u8 a;
    u16 m;

    if ((obj->input_bits_2 & 0x2000) == 0) {
        return 0;
    }
    m = *mask;
    a = (obj->input_bits_2 & 0x4000) ? 0 : 6;
    if (dir.second != 0) {
        a += 3;
    }
    a += dir.first >> 1;
    return m & a;
}
