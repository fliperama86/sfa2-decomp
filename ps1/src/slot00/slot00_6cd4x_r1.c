/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797f4_slot00[];
void func_80077050_slot00(Object *obj);

void func_80076cd4_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    int v;
    u8 t;
    int w;

    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 == 0) {
        o->field_00 = 1;
        o->field_04 = 1;
        o->field_06 = 1;
        o->field_05 = 0;
        o->field_07 = 0;
        v = -o->field_4c;
        o->field_4c = v;
        if (!(obj->field_b3 & 1)) {
            o->field_4c = (v >> 1) + v;
            if (o->field_ad == 0) {
                w = o->field_ae + 1;
                t = w;
                if (w >= 8) {
                    t = 7;
                }
                o->field_ae = t;
                func_80077050_slot00(o);
            }
        }
    }
    func_80131094(o);
}
