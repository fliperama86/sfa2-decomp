/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80029ed8_slot28[];
extern Object *data_8005187c_slot28[];
void func_80014854_slot28(Object *obj, int arg);

void func_800146dc_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        Object *o;
        h->countdown_timer = 0x258;
        h->pass_index += 1;
        o = data_8005187c_slot28[0];
        o->pos_y = -0x50;
        o->field_01 = 1;
        func_80130768(o, 1, data_80029ed8_slot28);
        func_80014854_slot28(obj, 5);
        func_80128370();
    }
}

void func_80014764_slot28(Object *obj) {
    if (obj->field_f0 == 0 && (s16)data_8005187c_slot28[3]->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index += 1;
    }
}

void func_800147b4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->pass_index += 1;
    }
}

void func_8001481c_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}
