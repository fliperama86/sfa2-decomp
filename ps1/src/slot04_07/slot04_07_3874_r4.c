/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b50_slot04_07(Object *obj) {
    int k;

    if (obj->field_cd == 0) {
        switch (((Slot04aObj *)obj)->field_1c8) {
        case 0:
            if (obj->field_130 == 0x8000) {
                ((Slot04aObj *)obj)->field_1c9 = 4;
                ((Slot04aObj *)obj)->field_1c8 = ((Slot04aObj *)obj)->field_1c8 + 1;
            }
            break;
        case 1:
            if (obj->field_130 == 0x1000) {
                ((Slot04aObj *)obj)->field_1c9 = 4;
                ((Slot04aObj *)obj)->field_1c8 = ((Slot04aObj *)obj)->field_1c8 + 1;
            } else {
                ((Slot04aObj *)obj)->field_1c9 = ((Slot04aObj *)obj)->field_1c9 - 1;
                if ((((Slot04aObj *)obj)->field_1c9 & 0x80) != 0) {
                    ((Slot04aObj *)obj)->field_1c8 = 0;
                }
            }
            break;
        case 2:
            if (obj->field_130 == 0x2000) {
                ((Slot04aObj *)obj)->field_1c9 = 4;
                ((Slot04aObj *)obj)->field_1c8 = ((Slot04aObj *)obj)->field_1c8 + 1;
            } else {
                ((Slot04aObj *)obj)->field_1c9 = ((Slot04aObj *)obj)->field_1c9 - 1;
                if ((((Slot04aObj *)obj)->field_1c9 & 0x80) != 0) {
                    ((Slot04aObj *)obj)->field_1c8 = 0;
                }
            }
            break;
        case 3:
            if ((obj->field_134 & 0x14) != 0) {
                obj->field_07 = obj->field_07 + 1;
                func_801307e0(obj, 0x18);
                return;
            }
            ((Slot04aObj *)obj)->field_1c9 = ((Slot04aObj *)obj)->field_1c9 - 1;
            if ((((Slot04aObj *)obj)->field_1c9 & 0x80) != 0) {
                ((Slot04aObj *)obj)->field_1c8 = 0;
            }
            break;
        }
    }
    if (*(u8 *)&obj->field_3a != 0) {
        func_80146478(obj, 1, -0x1a, 0x3a);
        func_80120554(obj, obj->side ^ 1, 0x30b);
        func_801204f4(obj, obj->side, 9);
        k = 0;
        obj->field_3a = obj->field_3a & 0xff00;
        if (((Slot04aObj *)obj)->field_1ce == 0) {
            ((Slot04aObj *)obj)->field_1ce = 1;
            k = 7;
        }
        if ((u8)func_80140cd8(obj, (s16)k, 0) != 0) {
            obj->field_07 = obj->field_07 + 3;
            func_801307e0(obj, 0x1a);
            return;
        }
        obj->field_46 = obj->field_46 + 0x100;
    }
    k = obj->field_134;
    if (k != 0) {
        if (*(s16 *)&obj->field_38 > 1) {
            obj->field_38 = *(s16 *)&obj->field_38 - 1;
        }
    }
    if ((*(s16 *)&obj->field_46 & 0xff00) == 0 || (obj->other->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 3;
        func_801307e0(obj, 0x1a);
    }
}
