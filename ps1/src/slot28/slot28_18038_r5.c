/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80028514_slot28(Object *obj) {
    obj->field_01 = 0;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_01 = 0;
    obj->field_04 = obj->field_04 + 1;
    func_80130768(obj, 3, (SequenceStep **)obj->box_tables);
}

void func_8002855c_slot28(Object *obj) {
    if (obj->pos_y < 0x201) {
        obj->field_01 = 1;
    }
    if (obj->pos_y < 0x83) {
        obj->field_48 = 0xff;
        obj->pos_y = 0x82;
    }
    func_80131094(obj);
}
