/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6584_slot04_09(Object *obj, Object *parent, int i);

void func_801b63c4_slot04_09(Object *obj, Object *parent) {
    int n;
    int c;
    int d;
    SequenceStep **t;
    obj->field_01 = 0;
    if (obj->field_03 != parent->kind) {
        obj->field_04++;
        return;
    }
    n = parent->frame->field_09;
    if (n == 0) {
        obj->field_48 = 0;
        return;
    }
    func_801b6584_slot04_09(obj, parent, n);
    obj->field_0c = parent->field_0c;
    obj->field_0d = parent->field_0d;
    obj->field_01 = 1;
    if (obj->field_48 == n) {
        d = obj->field_46 - 1;
        obj->field_46 = d;
        if ((s16)d < 0) {
            obj->field_46 = 0;
            obj->field_0c = parent->field_0c;
            obj->field_0d = parent->field_0d;
            c = obj->field_46 ^ 1;
            obj->field_46 = c;
            if (c & 1) {
                obj->field_0c = 0xff;
                obj->field_0d++;
            }
        }
        func_80131094(obj);
        if (((Slot04aObj *)obj)->field_3a == 1) {
            obj->field_0b = 0;
            obj->field_80 = 1;
        }
    } else {
        obj->field_48 = n;
        if (parent->side == 0) {
            t = data_1f8000b4;
        } else {
            t = data_1f800164;
        }
        func_80130768(obj, n, t);
        if (((Slot04aObj *)obj)->field_3a == 1) {
            obj->field_0b = 0;
        }
    }
}
