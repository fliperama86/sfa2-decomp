/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* t and a are 16-bit and t is assigned twice: with int locals the move is gone and the registers differ (3 slots). */
void func_801b504c_slot04_0e(Object *obj) {
    s16 t;
    s16 a;
    t = *(u8 *)&obj->field_3a;
    if (t != 0) {
        a = t & 0xff;
        t = a;
        if (obj->field_0b == 0) t = -a;
        *(u8 *)&obj->field_3a = 0;
        obj->pos_x = t + obj->pos_x;
    }
}
