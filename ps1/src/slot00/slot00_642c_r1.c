/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8007642c_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    if (obj->field_3a != 0) {
        o->field_07++;
        func_80120554(o, o->side, 0x324);
        if (o->field_0b != 0) {
            o->field_4c = 0xa0000;
            o->field_54 = -0x8000;
        } else {
            o->field_4c = 0xfff60000;
            o->field_54 = 0x8000;
        }
    }
    func_80130efc(o);
}

void func_800764a8_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    if (o->field_4c == 0) {
        o->field_07++;
        o->field_10 = 0;
    }
    if ((game_state.field_1d & 3) == 0) {
        if (func_80148e84(o) & 0xff) {
            if (o->field_0b == 0) {
                ref_other.p->pos_x -= 0x30;
            } else {
                ref_other.p->pos_x += 0x30;
            }
        }
    }
    func_80130efc(o);
}
