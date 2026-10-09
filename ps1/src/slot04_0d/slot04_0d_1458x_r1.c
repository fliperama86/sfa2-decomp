/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1540_slot04_0d(Object *obj);

extern u16 data_801c2768_slot04_0d[];
extern u8 data_801c2770_slot04_0d[];

void func_801b1458_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;
    u16 t;

    func_80130efc(o);
    if ((s16)o->field_3a >= 0) {
        a = o->field_134;
        if ((a & *(u16 *)((u8 *)data_801c2768_slot04_0d + (o->field_12a & 0xfe))) != 0) {
            obj->field_1ce = 1;
        }
    } else {
        if (obj->field_1ce == 0 || (obj->field_1ce = 0, obj->field_1cf == 2)) {
            func_801b1540_slot04_0d(o);
        } else {
            obj->field_1cf = obj->field_1cf + 1;
            func_80141f28(o, 1);
            t = data_801c2770_slot04_0d[obj->field_1cf];
            a = 0x28;
            if (o->field_49 != 0) {
                a = 0x46;
            }
            a += t;
            func_801307e0(o, (u16)a);
        }
    }
}
