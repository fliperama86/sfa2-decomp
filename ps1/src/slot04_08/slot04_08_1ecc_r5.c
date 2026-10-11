/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3c90_slot04_08[];

void func_801b22fc_slot04_08(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80145d20(obj);
    ((Slot04aObj *)obj)->field_1c6 = 0;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x2f);
}

void func_801b235c_slot04_08(Object *obj) {
    s16 t;
    u8 i;
    func_80130efc(obj);
    t = obj->step_flags;
    if ((t & 0x8000) == 0) {
        if ((u8)t != 0) {
            func_80120554(obj, obj->side, 0x31c);
            obj->step_flags = obj->step_flags & 0xff00;
            if (obj->field_4b == 0) {
                obj->field_165 = 0xff;
            } else {
                obj->field_165 = 1;
            }
            func_801483a4(obj, -0x10, 0x3c);
        }
    } else {
        obj->field_45 = 1;
        obj->state_3 = obj->state_3 + 1;
        i = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c3c90_slot04_08[i];
        if (obj->field_129 != 0) {
            obj->vel_x = 0xa8000;
        } else {
            obj->vel_x = 0x40000;
        }
        obj->accel_x = 0;
        obj->vel_y = -0xc0000;
        obj->accel_y = 0x12000;
        if (obj->field_0b == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
    }
}
