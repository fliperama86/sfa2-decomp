/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2048_slot04_0a(Object *obj) {
    int t;

    if (obj->field_cd == 0) {
        t = obj->field_130 & 0xa000;
        if (t != 0) {
            if ((t & 0x8000) == 0) {
                t = 3;
            } else {
                t = -3;
            }
            if (obj->field_0b != 0) {
                t = -t;
            }
            *(u16 *)&obj->pos_x += t;
        }
    }
}
