/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_80190544;

void func_801e9604_slot06_02(Object *obj);

/* e is a 16-bit local: as an int ten instruction slots differ, and with
   the first sum stored without it the function is 4 bytes shorter. */
void func_801e9530_slot06_02(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    s32 d;
    s16 e;

    if (game_state.field_65 == 0) {
        obj->field_30 -= 1;
        if (obj->field_30 == 0) {
            obj->field_88 = (obj->field_88 + 2) & 0x1e;
            func_801e9604_slot06_02(o);
        }
    }
    d = *(s32 *)&((Slot06Layer *)data_801aa5d4)->field_20;
    d -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->field_08;
    d += data_80190544;
    d += d / 16;
    d += obj->field_08;
    e = obj->field_36 + (d >> 16);
    obj->field_22 = e;
    e = ((Slot06Layer *)data_801aa5d4)->field_26;
    e -= ((Slot06Layer *)data_801aa5d4)->field_0e;
    e += obj->field_0e;
    e += obj->field_3a;
    obj->field_26 = e;
}
