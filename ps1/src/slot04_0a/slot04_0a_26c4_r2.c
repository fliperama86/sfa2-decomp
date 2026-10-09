/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f10_slot04_0a(Object *obj);

void func_801b2804_slot04_0a(Object *obj) {
    u8 t;
    Slot04aObj *s = (Slot04aObj *)obj;
    if (obj->field_67 != 0) {
        obj->field_67 = 0;
        t = s->field_104;
        if ((t & 0x80) == 0) {
            s->field_104 = t - 1;
            if (s->field_104 & 0x80) {
                func_801307e0(obj, 0x42);
            }
        }
    }
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 >= 0 && obj->field_0b != obj->field_164 - 1) {
        *(s32 *)&obj->field_10 += obj->field_4c;
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801b3f10_slot04_0a(obj);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x43);
    }
}

void func_801b28e0_slot04_0a(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        func_801312b8(obj);
    }
}
