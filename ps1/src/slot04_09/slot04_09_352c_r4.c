/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b3830_slot04_09(Object *obj) {
    Object *o;
    Object *p;
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    p = obj->other;
    if (obj->field_50 > 0) {
        if (p->field_45 != 0) {
            if (obj->field_a3 == 0) {
                if (obj->frame->box_a != 0) {
                    if ((u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x30) != 0) {
                        obj->field_a2 = 0xff;
                        obj->field_46 = 4;
                        obj->field_07++;
                        if (obj->field_4c >= 0) {
                            obj->field_4c = 0x80000;
                        } else {
                            obj->field_4c = -0x80000;
                        }
                        func_80120554(obj, obj->side ^ 1, 0x31a);
                        func_801204f4(obj, obj->side, 5);
                        func_801307e0(obj, 0x21);
                        return;
                    }
                }
            } else {
                if (func_8013fe54(obj, -0x20, 0x20, 0, 0x30) != 0) {
                    obj->field_a2 = 0xff;
                    obj->field_46 = 4;
                    obj->field_07++;
                    if (obj->field_4c >= 0) {
                        obj->field_4c = 0x80000;
                    } else {
                        obj->field_4c = -0x80000;
                    }
                    func_80120554(obj, obj->side ^ 1, 0x31a);
                    func_801204f4(obj, obj->side, 5);
                    func_801307e0(obj, 0x21);
                    return;
                }
            }
        }
        func_80130efc(obj);
    } else {
        o = obj;
        o->field_07 = 9;
        o->field_4c = 0;
        o->field_54 = 0;
        func_801307e0(o, 0x23);
    }
}
