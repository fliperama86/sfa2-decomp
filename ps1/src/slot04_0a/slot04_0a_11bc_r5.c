/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *obj);

extern s32 data_801c0608_slot04_0a[];

void func_801b1750_slot04_0a(Object *obj) {
    s32 v;
    s32 a;

    obj->state_3++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->active_state = 1;
    obj->active_flag_4 = 1;
    v = data_801c0608_slot04_0a[obj->direction_index >> 1];
    a = -0x2000;
    if (obj->facing_side == 0) {
        v = -v;
        a = 0x2000;
    }
    obj->vel_x = v;
    obj->vel_y = 0x20000;
    obj->accel_x = a;
    obj->accel_y = -0x3000;
    if (obj->field_49 == 0) {
        func_801307e0(obj, 0x1f);
    } else {
        func_801307e0(obj, 0x50);
    }
}

void func_801b1810_slot04_0a(Object *obj) {
    if (((Slot04aObj *)obj)->mode == 0) {
        func_801b3e00_slot04_0a(obj);
        if (obj->pos_y > obj->ground_y) {
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            obj->active_flag_4 = 0;
            obj->state_3++;
            obj->pos_y = obj->ground_y;
        }
    }
    update_sequence(obj);
}
