/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);

void func_801b1fb8_slot04_00(Object *obj) {
    if (obj->field_45 != 0) {
        func_801209c4(obj);
    }
    obj->field_07++;
    obj->field_14 = 0;
    obj->field_45 = 0;
    obj->field_159 = 0;
    obj->pos_y = obj->field_70;
    func_80130678(obj, 0x11);
}
