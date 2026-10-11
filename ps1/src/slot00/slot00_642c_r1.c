/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8007642c_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    if (obj->field_3a != 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x324);
        if (o->field_0b != 0) {
            o->vel_x = 0xa0000;
            o->accel_x = -0x8000;
        } else {
            o->vel_x = 0xfff60000;
            o->accel_x = 0x8000;
        }
    }
    update_sequence(o);
}

void func_800764a8_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->field_10 += o->vel_x;
    o->vel_x += o->accel_x;
    if (o->vel_x == 0) {
        o->state_3++;
        o->pos_x_frac = 0;
    }
    if ((game_state.field_1d & 3) == 0) {
        if (func_80148e84(o) & 0xff) {
            if (o->field_0b == 0) {
                ref_other.p->pos_x -= 0x30;
            } else {
                ref_other.p->pos_x += 0x30;
            }
        }
    }
    update_sequence(o);
}
