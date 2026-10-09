/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fe8(Object *object);

void func_801b351c_slot04_07(Object *obj) {
    int t;
    int u;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        t = (u8)obj->field_46;
        u = obj->field_07;
        obj->field_27c = 0;
        ((Slot04aObj *)obj)->field_27d = 0;
        u++;
        obj->field_07 = u;
        obj->field_46 = t | 0x1400;
        t = ((Slot04aObj *)obj)->field_c6 + 0x1e;
        if (obj->field_4b != 0) {
            t = 0x48;
        }
        obj->field_2a1 = t;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}
