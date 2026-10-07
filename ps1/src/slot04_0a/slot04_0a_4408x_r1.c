/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4408_slot04_0a(Object *obj, Object *parent) {
    int t;
    int f;
    u8 b;
    s16 x;

    obj->field_01 = 0;
    if (obj->field_03 != parent->kind) {
        obj->field_04++;
    } else {
        t = parent->frame->field_09 & 0x7f;
        if (t == 0) {
            obj->field_48 = 0;
        } else {
            f = obj->field_48;
            obj->pos_x = parent->pos_x;
            obj->pos_y = parent->pos_y;
            b = parent->field_0b;
            obj->field_01 = 1;
            obj->field_0b = b;
            if (f != t) {
                obj->field_46 = 3;
                obj->field_0d = parent->field_0d;
                obj->field_48 = t;
                if (parent->side == 0) {
                    func_80130768(obj, (s16)t, *(SequenceStep ***)0x1f8000b4);
                } else {
                    func_80130768(obj, (s16)t, *(SequenceStep ***)0x1f800164);
                }
            } else {
                if (((Slot04aObj *)obj)->field_3a == 0) {
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
}
