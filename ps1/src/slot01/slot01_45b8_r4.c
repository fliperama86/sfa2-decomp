/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80033ba8_slot01;
extern ObjectRef data_80033bac_slot01;
void func_80014b28_slot01(Object *obj, int arg);

void func_800149c8_slot01(Object *obj) {
    data_80033ba8_slot01.p = obj->field_3c;
    if (obj->field_03 != obj->field_48) {
        obj->field_03 = obj->field_48;
    }
    if (data_80033ba8_slot01.p->kind != obj->field_48) {
        obj->field_48 = data_80033ba8_slot01.p->kind;
        func_80014b28_slot01(obj, 0);
    } else if ((game_state.mode >> data_80033ba8_slot01.p->side) & 1) {
        obj->field_05++;
        obj->field_45 = data_80033ba8_slot01.p->field_d4;
        func_80014b28_slot01(obj, 1);
    }
}

void func_80014a8c_slot01(Object *obj) {
    Object *p = obj->field_3c;
    data_80033bac_slot01.p = p;
    if (p->field_d4 != obj->field_45) {
        obj->field_45 = p->field_d4;
    }
    if ((obj->field_3a << 16) < 0) {
        obj->field_5c = 0;
    }
    if (*(s16 *)&obj->field_5c == 0) {
        s16 t = *(s16 *)&obj->field_46;
        if (t != 0) {
            obj->field_46 = t - 1;
        }
    }
}
