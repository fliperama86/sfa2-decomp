/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);

void func_801b3654_slot04_09(Object *obj) {
    func_801b2858_slot04_09(obj);
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
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
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}
