/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd318_slot05_06[];
void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801c9514_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u8 i;

    if (obj->field_1c3 != 0) {
        if (obj->field_1c3 == 0x31) {
            func_80120554(o, o->side, 0x31c);
            func_801483a4(o, -0x26, 0x62);
        }
        obj->field_1c3--;
        if (obj->field_1c3 == 0) {
            i = 0;
            o->field_165 = 0;
            if (o->field_4b == 0) {
                o->other->field_6b = 0xa;
                i = (o->field_12a >> 1) + 1;
            }
            o->field_27b = data_801dd318_slot05_06[i];
        }
    } else {
        if (obj->field_3a != 0) {
            obj->field_1c3 = 10;
            o->field_07++;
        } else {
            func_80130efc(o);
        }
    }
}

void func_801c9600_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_801cc814_slot05_06(o);
    func_801cc84c_slot05_06(o);
    if (obj->field_1c3 != 0) {
        if (--obj->field_1c3 == 0) goto done;
    }
    if (o->pos_y < o->field_70) return;
done:
    o->field_45 = 0;
    o->field_249 = 5;
    o->field_07++;
    func_801209c4(o);
    func_80130efc(o);
}
