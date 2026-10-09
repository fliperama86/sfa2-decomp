/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* t and a are 16-bit locals: with int locals this function differs from the
   original in three instruction slots. */
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
