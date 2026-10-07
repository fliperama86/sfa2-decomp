/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c1d70_slot04_07[];

void func_801b414c_slot04_07(Object *obj) {
    u16 t = obj->field_3a;
    s16 x = t & 0x7f00;
    s16 v;

    if (x != 0) {
        obj->field_3a = t & 0x80ff;
        v = data_801c1d70_slot04_07[x >> 8];
        if ((v & 0xff00) != 0) {
            func_80120554(obj, obj->side, v);
        } else {
            func_801204f4(obj, obj->side, v);
        }
    }
}
