/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801ce6f8_slot05_06(Object *obj);
extern ObjectFn data_801dd6cc_slot05_06[];

void func_801ce4c0_slot05_06(Object *obj) {
    int a;
    int t;

    if ((func_801ce6f8_slot05_06(obj) << 16) > 0) {
        t = obj->state_1;
        a = obj->field_03;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        t++;
        a = (a & 1) | 0x18;
        obj->state_1 = t;
        if (obj->field_66 == 0) {
            func_80130768(obj, a, data_1f8000b4);
        } else {
            func_80130768(obj, a, data_1f800164);
        }
    }
    advance_sequence_checked(obj);
}

void func_801ce548_slot05_06(Object *obj) {
}

void func_801ce550_slot05_06(Object *obj) {
    GameState *g = &game_state;

    if ((func_801ce6f8_slot05_06(obj) << 16) >= 0) {
        obj->state_0++;
    }
    if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
        ((Slot04aObj *)obj)->timer -= 1;
        if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
            func_8011ffdc(obj);
            return;
        }
    }
    if (g->frame_counter & 1) {
        func_8011ffdc(obj);
    }
}

void func_801ce5e4_slot05_06(Object *o) {
    data_801dd6cc_slot05_06[o->state_1](o);
    func_8011ffdc(o);
}
