/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

void func_801b3654_slot04_09(Object *obj) {
    func_801b2858_slot04_09(obj);
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_67 == 0) {
            goto clear;
        }
        if (obj->other->field_61 == 0xff) {
            goto clear;
        }
        obj->field_a3 = 0xff;
        obj->field_67 = 0;
    }
    if ((u16)(obj->other->pos_x - obj->pos_x + 0x24) < 0x49) {
clear:
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
}
