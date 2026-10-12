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
            obj->table_index = (obj->table_index + 2) & 0x1e;
            func_801e9604_slot06_02(o);
        }
    }
    d = *(s32 *)&((Slot06Layer *)data_801aa5d4)->bound_x_c;
    d -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->bound_x_a;
    d += data_80190544;
    d += d / 16;
    d += obj->fixed_offset;
    e = obj->base_value + (d >> 16);
    obj->computed_value = e;
    e = ((Slot06Layer *)data_801aa5d4)->field_26;
    e -= ((Slot06Layer *)data_801aa5d4)->field_0e;
    e += obj->offset_a;
    e += obj->offset_b;
    obj->parallax_factor = e;
}
