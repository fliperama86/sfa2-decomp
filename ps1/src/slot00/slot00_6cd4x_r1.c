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
        o->state_0 = 1;
        o->state_2 = 1;
        o->state_1 = 0;
        o->state_3 = 0;
        v = -o->vel_x;
        o->vel_x = v;
        if (!(obj->field_b3 & 1)) {
            o->vel_x = (v >> 1) + v;
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
