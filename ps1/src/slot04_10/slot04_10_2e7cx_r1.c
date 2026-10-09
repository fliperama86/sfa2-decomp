/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2e7c_slot04_10(Object *obj);
void func_801b2f98_slot04_10(Object *obj);

void func_801b2e7c_slot04_10(Object *obj) {
    s32 a1;

    if ((s16)obj->field_3a != 0) {
        obj->field_17b = 0;
        obj->field_07 = obj->field_07 + 1;
        func_801b2f98_slot04_10(obj);
        return;
    }
    if (obj->field_cd == 0) {
        a1 = obj->field_4c;
        if (obj->field_c2 & 0x8000) {
        } else if (obj->field_c2 & 0x2000) {
            a1 = -a1;
        } else {
            a1 = 0;
        }
        *(s32 *)&obj->field_10 += a1;
        func_80130efc(obj);
    } else {
        if (((Slot04bObj *)obj)->field_1a9 == 0) {
            func_80130efc(obj);
        } else if (((Slot04bObj *)obj)->field_1aa != 0) {
            ((Slot04bObj *)obj)->field_1aa--;
            func_80130efc(obj);
        } else {
            if (((Slot04bObj *)obj)->field_1ab == 0) {
                func_80130efc(obj);
            }
            a1 = obj->field_4c;
            ((Slot04bObj *)obj)->field_1ab--;
            if (((Slot04bObj *)obj)->field_1a9 & 0x80) {
                a1 = -a1;
            }
            if (obj->field_0b != 0) {
                a1 = -a1;
            }
            *(s32 *)&obj->field_10 += a1;
            func_80130efc(obj);
        }
    }
}
