/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_801c2090_slot04_07[];
extern int data_801c209c_slot04_07[];

void func_801b3874_slot04_07(Object *obj);

void func_801b36d4_slot04_07(Object *obj) {
    int a;
    int b;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    a = data_801c2090_slot04_07[obj->direction_index >> 1];
    obj->accel_x = -0x10000;
    obj->variant_index = obj->facing_side;
    obj->facing_side = obj->facing_side ^ 1;
    obj->vel_x = a;
    b = 0x60;
    if (obj->field_49 != 0) {
        b = 0x63;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + b);
}

void func_801b3784_slot04_07(Object *obj) {
    int a;

    update_sequence(obj);
    func_801b3874_slot04_07(obj);
    if (((Slot04aObj *)obj)->mode == 0) {
        obj->state_3++;
        obj->facing_side = obj->facing_side ^ 1;
        a = data_801c209c_slot04_07[obj->direction_index >> 1];
        obj->accel_x = -0x10000;
        obj->vel_x = a;
    }
}

void func_801b3804_slot04_07(Object *obj) {
    func_801b3874_slot04_07(obj);
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
        if (((Slot04aObj *)obj)->mode != 0) {
            obj->active_flag_4 = 0;
        }
    }
}
