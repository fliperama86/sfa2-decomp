/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b4cac_slot04_0f(Object *obj, u16 *mask, BytePair dir);

int func_801b4cac_slot04_0f(Object *obj, u16 *mask, BytePair dir) {
    u8 a;
    u16 m;

    if ((obj->field_130 & 0x2000) == 0) {
        return 0;
    }
    m = *mask;
    a = (obj->field_130 & 0x4000) ? 0 : 6;
    if (dir.second != 0) {
        a += 3;
    }
    a += dir.first >> 1;
    return m & a;
}
