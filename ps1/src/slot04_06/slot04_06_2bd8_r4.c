/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);

void func_801b3084_slot04_06(Object *obj) {
    int v;

    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 8);
    func_80138ae8(&game_state, obj);
    obj->vel_x = 0x40000;
    obj->vel_y = 0x28000;
    obj->accel_x = 0;
    obj->accel_y = 0xffff6000;
    obj->active_state = 1;
    obj->active_flag_3 = 1;
    v = 0x48;
    if (obj->field_49 == 0) {
        v = 0x2a;
    }
    func_801307e0(obj, v);
}

void func_801b3134_slot04_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        ((Slot04aObj *)obj)->countdown_2 = 0x10;
    } else {
        update_sequence(obj);
    }
}

void func_801b3178_slot04_06(Object *obj) {
    func_801b4818_slot04_06(obj);
    func_801b4850_slot04_06(obj);
    if (((Slot04aObj *)obj)->countdown_2 != 0) {
        if (--((Slot04aObj *)obj)->countdown_2 == 0) goto go;
    }
    if (obj->pos_y > obj->ground_y) {
go:
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        update_sequence(obj);
    }
}
