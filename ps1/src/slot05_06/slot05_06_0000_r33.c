/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cab84_slot05_06(Object *obj);

void func_801caa3c_slot05_06(Object *o) {
    ((Slot04aObj *)o)->field_1c2 += 0xff;
    if (((Slot04aObj *)o)->field_1c2 & 0x80) {
        o->state_3++;
    }
    update_sequence(o);
}

void func_801caa88_slot05_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;
    Object *other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else if (obj->field_12a == 4) {
        func_801cab84_slot05_06(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        other->field_260 = 1;
        func_80140770(obj, 2, 0xf, -0x200, 0, 0, 0);
        if ((s16)other->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        obj->vel_x = -0x20000;
        obj->vel_y = 0x44000;
        obj->accel_x = 0;
        obj->accel_y = -0x4800;
        obj->field_45 = 1;
        obj->state_3 = obj->state_3 + 1;
        func_801307e0(obj, 0x26);
    }
}

void func_801cab84_slot05_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    obj->state_3 = 0x13;
    obj->vel_x = 0x80000;
    obj->accel_x = 0;
    play_sound_at((Object *)&obj->other, s->field_e6, 0x34e);
    func_801307e0(obj, 0x38);
}
