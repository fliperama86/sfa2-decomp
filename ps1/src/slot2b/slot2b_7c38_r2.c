/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079954_slot2b[];
extern s32 data_8007995c_slot2b[];

void func_80077d68_slot2b(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_80079954_slot2b[o->state_2](o);
    }
    func_8011ffdc(o);
}

void func_80077dd4_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s8 v;
    s->field_47 = 2;
    o->state_2++;
    o->field_0b ^= 1;
    o->field_67 = 0;
    func_801380f0(o);
    v = s->field_a0;
    if (v & 0x80) {
        advance_sequence_checked(o);
    } else {
        func_80138070(o, v);
    }
}

void func_80077e50_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_47 += 0xff;
    if (s->field_47 & 0x80) {
        o->field_00 = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->vel_x = -o->vel_x;
        o->vel_y = data_8007995c_slot2b[o->field_ac >> 1];
    }
    advance_sequence_checked(o);
}
