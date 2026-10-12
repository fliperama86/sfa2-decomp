/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3d48_slot04_08[];
extern u16 data_801c3e98_slot04_08;

void func_801b4298_slot04_08(Object *obj);
void func_801b2cec_slot04_08(Object *obj);

void func_801b2bf8_slot04_08(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x31c);
        func_801483a4(obj, -0xa, 0x72);
    }
    if (obj->step_flags != 0) {
        u8 i = 0;

        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c3d48_slot04_08[i];
        func_801307e0(obj, (obj->direction_index >> 1) + 0x55);
        func_801b2cec_slot04_08(obj);
    }
}

void func_801b2cec_slot04_08(Object *obj) {
    int a;

    if ((u8)obj->step_flags != 0) {
        obj->vel_x = 0x90000;
        obj->accel_x = -0x4000;
        obj->state_3++;
        obj->active_flag = 0;
        a = 0xc0;
        if (obj->facing_side == 0) {
            a = -0xc0;
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        data_801c3e98_slot04_08 = a + obj->pos_x;
        obj->countdown = 0;
        func_801b4298_slot04_08(obj);
    }
    update_sequence(obj);
}
