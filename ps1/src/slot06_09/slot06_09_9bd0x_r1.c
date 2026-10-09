/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9cc0_slot06_09(Object *obj);

void func_801e9bd0_slot06_09(Object *o) {
    Slot06Layer *l2 = (Slot06Layer *)data_801aa5d4;
    Slot06Obj *p;
    s16 d;

    if (game_state.field_65 == 0) {
        o->field_46 = (s16)o->field_46 - 1;
        if ((s16)o->field_46 == 0) {
            o->field_4c += 4;
            if (o->field_4c >= 0x20) {
                o->field_4c = 0;
            }
            func_801e9cc0_slot06_09(o);
        }
    }
    d = l2->field_0a;
    d -= l2->field_22;
    d -= d >> 2;
    p = (Slot06Obj *)o;
    d += p->field_54;
    o->pos_x = d;
    d = l2->field_0e;
    d -= l2->field_26;
    d += p->field_58;
    o->pos_y = 0xf0 - d;
    func_80120028(o);
}
