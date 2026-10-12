/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3df4_slot04_08[])(Object *, Object *);

/* The call of data_801c3df4_slot04_08 passes one argument although its entries take two: the original does not set the second argument register before it. Written with a second parameter passed on, this function differs from the original in 5 instruction slots. */
void func_801b48d8_slot04_08(Object *obj) {
    obj->field_157 = 1;
    ((void (*)(Object *))data_801c3df4_slot04_08[obj->state_3])(obj);
}

void func_801b491c_slot04_08(Object *obj, Object *unused) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}

void func_801b4954_slot04_08(Object *obj, Object *unused) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
