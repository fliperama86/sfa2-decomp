/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 *data_801c2060_slot04_07[];

void func_801b3120_slot04_07(Object *obj);

void func_801b3054_slot04_07(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        func_80131468(obj);
    }
}

void func_801b309c_slot04_07(Object *obj) {
    int a;

    obj->state_3++;
    obj->active_flag = 0;
    ((Slot04aObj *)obj)->seen_flag = 0;
    a = obj->direction_index >> 1;
    a = data_801c2060_slot04_07[a][(s16)obj->countdown >> 1];
    if (a >= 0) {
        func_801307e0(obj, a);
    } else {
        func_801b3120_slot04_07(obj);
    }
}

void func_801b3120_slot04_07(Object *obj) {
    obj->state_3 = 7;
    obj->vel_y = 0;
    obj->accel_y = -0x5600;
    obj->vel_x = 0;
    obj->accel_x = 0;
    set_sequence_by_side(obj, 0x21);
}
