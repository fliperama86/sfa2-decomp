/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The cast of t to s16 in the call of func_80130768: without it this function differs from the original in 9 instruction slots. The local x is decremented in a statement of its own: written x = parent->field_46 - 1, 2 instruction slots differ. The local t is reused for the sum with field_0d: with a local of its own, 21 instruction slots differ. The sum is formed as t += v, then stored: written as t + v in the store, 12 instruction slots differ. */
void func_801b4408_slot04_0a(Object *obj, Object *parent) {
    int t;
    s16 x;

    obj->field_01 = 0;
    if (obj->field_03 != parent->kind) {
        obj->field_04++;
    } else {
        t = parent->frame->field_09 & 0x7f;
        if (t == 0) {
            obj->field_48 = 0;
        } else {
            obj->pos_x = parent->pos_x;
            obj->pos_y = parent->pos_y;
            obj->field_0b = parent->field_0b;
            obj->field_01 = 1;
            if (obj->field_48 != t) {
                obj->field_46 = 3;
                obj->field_0d = parent->field_0d;
                obj->field_48 = t;
                if (parent->side == 0) {
                    func_80130768(obj, (s16)t, data_1f8000b4);
                } else {
                    func_80130768(obj, (s16)t, data_1f800164);
                }
            } else if (((Slot04aObj *)obj)->field_3a == 0) {
                x = parent->field_46;
                x--;
                parent->field_46 = x;
                if (x == 0) {
                    int v;

                    t = parent->field_0d;
                    v = obj->field_4c ^ 1;
                    obj->field_46 = 3;
                    obj->field_4c = v;
                    t += v;
                    obj->field_0d = t;
                }
                func_80131094(obj);
            } else {
                obj->field_0d = parent->field_0d;
                func_80131094(obj);
            }
        }
    }
}
