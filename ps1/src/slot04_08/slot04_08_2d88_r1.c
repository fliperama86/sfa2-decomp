/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4298_slot04_08(Object *obj);
extern u16 data_801c3e98_slot04_08;

void func_801b2d88_slot04_08(Object *obj) {
    s16 d;
    u16 lim = 0x80;
    u16 t;
    Object *other;

    t = obj->field_46 + 1;
    t &= 7;
    obj->field_46 = t;
    if (t == 0) {
        func_801b4298_slot04_08(obj);
    }
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    other = obj->other;
    d = (u16)obj->pos_x - data_801c3e98_slot04_08;
    if (obj->field_0b != 0) {
        d = -d;
    }
    if (d >= 0) {
        d = 2;
        if (obj->field_4c < 0) {
            d = 1;
        }
        if (obj->field_164 != d && obj->field_67 == 0) {
            d = -0x20;
            if (obj->field_0b != 0) {
                d = 0x20;
            }
            d = d + (u16)obj->pos_x - other->pos_x + 0x40;
            if (lim < (u16)d) {
                func_80130efc(obj);
                return;
            }
        }
    }
    obj->field_07++;
    func_801307e0(obj, ((Slot04aObj *)obj)->field_1c4 + 0x46 + (obj->field_12a >> 1) * 3);
}
