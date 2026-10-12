/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007994c_slot2b[];

void func_80077c38_slot2b(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_8007994c_slot2b[o->state_2](o);
    }
    func_8011ffdc(o);
}

void func_80077ca4_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_47 = 2;
    o->state_2++;
    o->field_0b ^= 1;
    o->field_67 = 0;
    func_801380f0(o);
    func_80138070(o, (o->field_ac >> 1) + 6);
}

void func_80077d04_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_47--;
    if (s->field_47 == 0) {
        o->field_00 = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->vel_y = 0;
        o->vel_x = -o->vel_x;
    }
    advance_sequence_checked(o);
}
