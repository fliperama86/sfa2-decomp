/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f58_slot04_0e(Object *obj);
void func_801b3284_slot04_0e(Object *obj);

void func_801b2f58_slot04_0e(Object *obj) {
    obj->state_3 = 6;
    obj->field_45 = 0;
    obj->pos_y_frac = 0;
    obj->field_17b = 0;
    obj->pos_y = (u16)obj->field_70;
    func_801209c4(obj);
    func_801307e0(obj, 0x30);
}

void func_801b2fa8_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_1de == 0) {
        func_801b3284_slot04_0e(o);
    }
    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        o->vel_x = 0x60000;
        o->vel_y = 0x90000;
        o->accel_y = -0x8000;
        o->state_3++;
        o->accel_x = 0;
        if (o->field_0b == 0) {
            o->vel_x = -o->vel_x;
        }
        func_801307e0(o, 0x2f);
    }
}
