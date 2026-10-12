/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd404_slot05_06[];

void func_801ca568_slot05_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->vel_x = 0x18000;
        obj->vel_y = 0x8a000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
        obj->state_3 = obj->state_3 + 1;
        func_80140cd8(obj, *(s16 *)(data_801dd404_slot05_06 + (obj->direction_index & 0xfe)), 0);
        func_801307e0(obj, 0x23);
    } else {
        update_sequence(obj);
    }
}

void func_801ca610_slot05_06(Object *o) {
    if (*(u8 *)&o->step_flags != 0) {
        o->active_state = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->state_3++;
    }
    update_sequence(o);
}
