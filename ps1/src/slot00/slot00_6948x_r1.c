/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800797dc_slot00[];
void func_80077050_slot00(Object *obj);
void func_800770b8_slot00(Object *obj);
void func_80077150_slot00(Object *obj);

void func_80076948_slot00(Object *o) {
    unsigned i;
    int lim;
    func_800770b8_slot00(o);
    i = o->field_ad != 0;
    i = (-i & 6) + o->field_ac;
    i >>= 1;
    lim = data_800797dc_slot00[i];
    o->field_46++;
    if ((s16)o->field_46 >= lim) {
        o->field_06++;
        func_80138070(o, o->field_ad);
        func_80077050_slot00(o);
    } else {
        ref_other.p = o->field_3c;
        if (((Slot00Obj *)o)->field_a4 != ((ref_other.p->field_04 << 24) | (ref_other.p->field_05 << 16) | (ref_other.p->field_06 << 8))) {
            o->field_04 = 3;
            o->field_05 = 0;
            o->field_06 = 0;
            o->field_07 = 0;
        }
        if (game_state.field_47 != 0) {
            o->field_00 = 2;
        }
        ((Slot00Obj *)o)->field_b2--;
        if (((Slot00Obj *)o)->field_b2 & 0x80) {
            ((Slot00Obj *)o)->field_b2 = 1;
            func_80077150_slot00(o);
        }
        func_80131094(o);
    }
}
