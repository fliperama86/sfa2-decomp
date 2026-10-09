/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b64c4_slot04_06(Object *obj) {
    int a;
    int t;

    if ((func_801b66fc_slot04_06(obj) << 16) > 0) {
        t = obj->field_05;
        a = obj->field_03;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        t++;
        a = (a & 1) | 0x18;
        obj->field_05 = t;
        if (obj->field_66 == 0) {
            func_80130768(obj, a, data_1f8000b4);
        } else {
            func_80130768(obj, a, data_1f800164);
        }
    }
    func_80131094(obj);
}
