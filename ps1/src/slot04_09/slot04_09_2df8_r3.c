/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b3040_slot04_09(Object *obj) {
    Object *p;

    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    p = obj->other;
    if (obj->field_50 > 0) {
        if (p->field_163 == 0 && p->field_45 != 0 && (u8)func_8013fd98(obj, -0x20, 0x20, 0, 0x30) != 0) {
            obj->field_a2 = 0xff;
            obj->field_46 = 4;
            obj->field_07++;
            if (obj->field_4c >= 0) {
                obj->field_4c = 0x80000;
            } else {
                obj->field_4c = 0xfff80000;
            }
            func_80120554(obj, obj->side ^ 1, 0x31a);
            func_801204f4(obj, obj->side, 5);
            func_801307e0(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        obj->field_07 = 7;
        obj->field_4c = 0;
        obj->field_54 = 0;
        func_801307e0(obj, 0x23);
    }
}
