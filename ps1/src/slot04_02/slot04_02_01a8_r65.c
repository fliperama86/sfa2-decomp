/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7808_slot04_02[];
void func_801b7920_slot04_02(Object *obj, u8 a);
void func_801b7908_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b7384_slot04_02(Object *obj) {
    u8 a;

    obj->countdown = 2;
    obj->state_2++;
    obj->facing_side ^= 1;
    func_801380f0(obj);
    a = data_801c7808_slot04_02[obj->vel_table_index];
    if (a != 0) {
        func_801b7920_slot04_02(obj, a);
    }
    advance_sequence_checked(obj);
}

void func_801b7400_slot04_02(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->active_count = 1;
        func_801b7908_slot04_02(obj, 1, 0, 0, 0);
        obj->vel_x = -obj->vel_x;
        obj->vel_y = 0;
    }
    advance_sequence_checked(obj);
}
