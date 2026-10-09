/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void func_801204f4(Object *o, int b, int c);
void func_80130efc(Object *object);

/* functions of other units of this module */

void func_801b0ef4_slot04_16(Object *obj) {
    s16 t;

    if ((s16)obj->field_3a < 0 && *(u8 *)&((Slot04bObj *)obj)->field_330 == 3 && (s16)obj->field_46 < 0x1e) {
        func_801204f4(obj, obj->side, 0xe);
        ((Slot04bObj *)obj)->field_330 = 0;
    }
    t = obj->field_46;
    if (t == 0) {
        func_80130efc(obj);
    } else {
        t -= 1;
        obj->field_46 = t;
        if (t != 0) {
            func_80130efc(obj);
        } else {
            game_state.config->field_4b |= 1 << obj->side;
            func_80130efc(obj);
        }
    }
}
