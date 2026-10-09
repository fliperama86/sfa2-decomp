/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1548_slot04_15(Object *obj);

extern u8 data_801c2dec_slot04_15[];
extern u8 data_801c2df4_slot04_15[];

void func_801b1460_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    func_80130efc(o);
    if ((s16)o->field_3a >= 0) {
        a = o->field_134;
        if ((a & *(u16 *)(data_801c2dec_slot04_15 + (o->field_12a & 0xfe))) != 0) {
            obj->field_1ce = 1;
        }
    } else {
        if (obj->field_1ce == 0 || (obj->field_1ce = 0, obj->field_1cf == 2)) {
            func_801b1548_slot04_15(o);
        } else {
            u16 t;

            obj->field_1cf = obj->field_1cf + 1;
            func_80141f28(o, 1);
            t = data_801c2df4_slot04_15[obj->field_1cf];
            a = 0x28;
            if (o->field_49 != 0) {
                a = 0x46;
            }
            a += t;
            func_801307e0(o, (u16)a);
        }
    }
}
