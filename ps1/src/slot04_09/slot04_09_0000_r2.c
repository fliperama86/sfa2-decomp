/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5588_slot04_09(Object *obj);

void func_801b019c_slot04_09(Object *obj) {
    obj->field_45 = 1;
    obj->field_06 = obj->field_06 + 1;
    func_801b5588_slot04_09(obj);
    func_801204f4(obj, obj->side, 0x10);
    func_80130678(obj, 0x22);
}

void func_801b01f4_slot04_09(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->pos_y = obj->field_70;
        func_80130678(obj, 0);
    } else {
        func_80130efc(obj);
    }
}
